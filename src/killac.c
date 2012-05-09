/* ========================================================================== */
/*   Killa compiler (saves bytecodes to files; also list bytecodes)           */
/* -------------------------------------------------------------------------- */
/*   Copyright (c) 2012 Laurens Rodriguez Oscanoa.                            */
/*   Copyright (C) 1994-2012 Lua.org, PUC-Rio.                                */
/*                                                                            */
/*   This code is licensed under the MIT license:                             */
/*   http://www.opensource.org/licenses/mit-license.php                       */
/* -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KILLA_CORE

#include "killa.h"
#include "kauxlib.h"

#include "kobject.h"
#include "kstate.h"
#include "kundump.h"

static void PrintFunction(const killa_Proto* f, int full);
#define luaU_print	PrintFunction

#define PROGNAME	"luac"		/* default program name */
#define OUTPUT		PROGNAME ".out"	/* default output file */

static int listing=0;			/* list bytecodes? */
static int dumping=1;			/* dump bytecodes? */
static int stripping=0;			/* strip debug information? */
static char Output[]={ OUTPUT };	/* default output file name */
static const char* output=Output;	/* actual output file name */
static const char* progname=PROGNAME;	/* actual program name */

static void fatal(const char* message)
{
 fprintf(stderr,"%s: %s\n",progname,message);
 exit(EXIT_FAILURE);
}

static void cannot(const char* what)
{
 fprintf(stderr,"%s: cannot %s %s: %s\n",progname,what,output,strerror(errno));
 exit(EXIT_FAILURE);
}

static void usage(const char* message)
{
 if (*message=='-')
  fprintf(stderr,"%s: unrecognized option " KILLA_QS "\n",progname,message);
 else
  fprintf(stderr,"%s: %s\n",progname,message);
 fprintf(stderr,
  "usage: %s [options] [filenames]\n"
  "Available options are:\n"
  "  -l       list (use -l -l for full listing)\n"
  "  -o name  output to file " KILLA_QL("name") " (default is \"%s\")\n"
  "  -p       parse only\n"
  "  -s       strip debug information\n"
  "  -v       show version information\n"
  "  --       stop handling options\n"
  "  -        stop handling options and process stdin\n"
  ,progname,Output);
 exit(EXIT_FAILURE);
}

#define IS(s)	(strcmp(argv[i],s)==0)

static int doargs(int argc, char* argv[])
{
 int i;
 int version=0;
 if (argv[0]!=NULL && *argv[0]!=0) progname=argv[0];
 for (i=1; i<argc; i++)
 {
  if (*argv[i]!='-')			/* end of options; keep it */
   break;
  else if (IS("--"))			/* end of options; skip it */
  {
   ++i;
   if (version) ++version;
   break;
  }
  else if (IS("-"))			/* end of options; use stdin */
   break;
  else if (IS("-l"))			/* list */
   ++listing;
  else if (IS("-o"))			/* output file */
  {
   output=argv[++i];
   if (output==NULL || *output==0 || (*output=='-' && output[1]!=0))
    usage(KILLA_QL("-o") " needs argument");
   if (IS("-")) output=NULL;
  }
  else if (IS("-p"))			/* parse only */
   dumping=0;
  else if (IS("-s"))			/* strip debug information */
   stripping=1;
  else if (IS("-v"))			/* show version */
   ++version;
  else					/* unknown option */
   usage(argv[i]);
 }
 if (i==argc && (listing || !dumping))
 {
  dumping=0;
  argv[--i]=Output;
 }
 if (version)
 {
  printf("%s\n",KILLA_COPYRIGHT);
  if (version==argc-1) exit(EXIT_SUCCESS);
 }
 return i;
}

#define FUNCTION "(function()end)();"

static const char* reader(killa_State *L, void *ud, size_t *size)
{
 KILLA_UNUSED(L);
 if ((*(int*)ud)--)
 {
  *size=sizeof(FUNCTION)-1;
  return FUNCTION;
 }
 else
 {
  *size=0;
  return NULL;
 }
}

#define toproto(L,i) killa_getproto(L->top+(i))

static const killa_Proto* combine(killa_State* L, int n)
{
 if (n==1)
  return toproto(L,-1);
 else
 {
  killa_Proto* f;
  int i=n;
  if (killa_load(L,reader,&i,"=(" PROGNAME ")",NULL)!=KILLA_OK) fatal(killa_tostring(L,-1));
  f=toproto(L,-1);
  for (i=0; i<n; i++)
  {
   f->p[i]=toproto(L,i-n-1);
   if (f->p[i]->sizeupvalues>0) f->p[i]->upvalues[0].instack=0;
  }
  f->sizelineinfo=0;
  return f;
 }
}

static int writer(killa_State* L, const void* p, size_t size, void* u)
{
 KILLA_UNUSED(L);
 return (fwrite(p,size,1,(FILE*)u)!=1) && (size!=0);
}

static int pmain(killa_State* L)
{
 int argc=(int)killa_tointeger(L,1);
 char** argv=(char**)killa_touserdata(L,2);
 const killa_Proto* f;
 int i;
 if (!killa_checkstack(L,argc)) fatal("too many input files");
 for (i=0; i<argc; i++)
 {
  const char* filename=IS("-") ? NULL : argv[i];
  if (killaL_loadfile(L,filename)!=KILLA_OK) fatal(killa_tostring(L,-1));
 }
 f=combine(L,argc);
 if (listing) luaU_print(f,listing>1);
 if (dumping)
 {
  FILE* D= (output==NULL) ? stdout : fopen(output,"wb");
  if (D==NULL) cannot("open");
  killa_lock(L);
  killaU_dump(L,f,writer,D,stripping);
  killa_unlock(L);
  if (ferror(D)) cannot("write");
  if (fclose(D)) cannot("close");
 }
 return 0;
}

int main(int argc, char* argv[])
{
 killa_State* L;
 int i=doargs(argc,argv);
 argc-=i; argv+=i;
 if (argc<=0) usage("no input files given");
 L=killaL_newstate();
 if (L==NULL) fatal("cannot create state: not enough memory");
 killa_pushcfunction(L,&pmain);
 killa_pushinteger(L,argc);
 killa_pushlightuserdata(L,argv);
 if (killa_pcall(L,2,0,0)!=KILLA_OK) fatal(killa_tostring(L,-1));
 killa_close(L);
 return EXIT_SUCCESS;
}

/*
** $Id: print.c,v 1.68 2011/09/30 10:21:20 lhf Exp $
** print bytecodes
** See Copyright Notice in lua.h
*/

#include <ctype.h>
#include <stdio.h>

#define luac_c
#define KILLA_CORE

#include "kdebug.h"
#include "kobject.h"
#include "kopcodes.h"

#define VOID(p)		((const void*)(p))

static void PrintString(const killa_TString* ts)
{
 const char* s=killa_getstr(ts);
 size_t i,n=ts->tsv.len;
 printf("%c",'"');
 for (i=0; i<n; i++)
 {
  int c=(int)(unsigned char)s[i];
  switch (c)
  {
   case '"':  printf("\\\""); break;
   case '\\': printf("\\\\"); break;
   case '\a': printf("\\a"); break;
   case '\b': printf("\\b"); break;
   case '\f': printf("\\f"); break;
   case '\n': printf("\\n"); break;
   case '\r': printf("\\r"); break;
   case '\t': printf("\\t"); break;
   case '\v': printf("\\v"); break;
   default:	if (isprint(c))
   			printf("%c",c);
		else
			printf("\\%03d",c);
  }
 }
 printf("%c",'"');
}

static void PrintConstant(const killa_Proto* f, int i)
{
 const killa_TValue* o=&f->k[i];
 switch (killa_ttype(o))
 {
  case KILLA_TNULL:
	printf("nil");
	break;
  case KILLA_TBOOLEAN:
	printf(killa_bvalue(o) ? "true" : "false");
	break;
  case KILLA_TNUMBER:
	printf(KILLA_NUMBER_FMT,killa_nvalue(o));
	break;
  case KILLA_TSTRING:
	PrintString(killa_rawtsvalue(o));
	break;
  default:				/* cannot happen */
	printf("? type=%d",killa_ttype(o));
	break;
 }
}

#define UPVALNAME(x) ((f->upvalues[x].name) ? killa_getstr(f->upvalues[x].name) : "-")
#define MYK(x)		(-1-(x))

static void PrintCode(const killa_Proto* f)
{
 const killa_Instruction* code=f->code;
 int pc,n=f->sizecode;
 for (pc=0; pc<n; pc++)
 {
  killa_Instruction i=code[pc];
  killa_OpCode o=KILLA_GET_OPCODE(i);
  int a=KILLA_GETARG_A(i);
  int b=KILLA_GETARG_B(i);
  int c=KILLA_GETARG_C(i);
  int ax=KILLA_GETARG_Ax(i);
  int bx=KILLA_GETARG_Bx(i);
  int sbx=KILLA_GETARG_sBx(i);
  int line=killa_getfuncline(f,pc);
  printf("\t%d\t",pc+1);
  if (line>0) printf("[%d]\t",line); else printf("[-]\t");
  printf("%-9s\t",killaP_opnames[o]);
  switch (killa_getOpMode(o))
  {
   case iABC:
    printf("%d",a);
    if (killa_getBMode(o)!=OpArgN) printf(" %d",KILLA_ISK(b) ? (MYK(KILLA_INDEXK(b))) : b);
    if (killa_getCMode(o)!=OpArgN) printf(" %d",KILLA_ISK(c) ? (MYK(KILLA_INDEXK(c))) : c);
    break;
   case iABx:
    printf("%d",a);
    if (killa_getBMode(o)==OpArgK) printf(" %d",MYK(bx));
    if (killa_getBMode(o)==OpArgU) printf(" %d",bx);
    break;
   case iAsBx:
    printf("%d %d",a,sbx);
    break;
   case iAx:
    printf("%d",MYK(ax));
    break;
  }
  switch (o)
  {
   case OP_LOADK:
    printf("\t; "); PrintConstant(f,bx);
    break;
   case OP_GETUPVAL:
   case OP_SETUPVAL:
    printf("\t; %s",UPVALNAME(b));
    break;
   case OP_GETTABUP:
    printf("\t; %s",UPVALNAME(b));
    if (KILLA_ISK(c)) { printf(" "); PrintConstant(f,KILLA_INDEXK(c)); }
    break;
   case OP_SETTABUP:
    printf("\t; %s",UPVALNAME(a));
    if (KILLA_ISK(b)) { printf(" "); PrintConstant(f,KILLA_INDEXK(b)); }
    if (KILLA_ISK(c)) { printf(" "); PrintConstant(f,KILLA_INDEXK(c)); }
    break;
   case OP_GETTABLE:
   case OP_SELF:
    if (KILLA_ISK(c)) { printf("\t; "); PrintConstant(f,KILLA_INDEXK(c)); }
    break;
   case OP_SETTABLE:
   case OP_ADD:
   case OP_SUB:
   case OP_MUL:
   case OP_DIV:
   case OP_POW:
   case OP_EQ:
   case OP_LT:
   case OP_LE:
    if (KILLA_ISK(b) || KILLA_ISK(c))
    {
     printf("\t; ");
     if (KILLA_ISK(b)) PrintConstant(f,KILLA_INDEXK(b)); else printf("-");
     printf(" ");
     if (KILLA_ISK(c)) PrintConstant(f,KILLA_INDEXK(c)); else printf("-");
    }
    break;
   case OP_JMP:
   case OP_FORLOOP:
   case OP_FORPREP:
   case OP_TFORLOOP:
    printf("\t; to %d",sbx+pc+2);
    break;
   case OP_CLOSURE:
    printf("\t; %p",VOID(f->p[bx]));
    break;
   case OP_SETLIST:
    if (c==0) printf("\t; %d",(int)code[++pc]); else printf("\t; %d",c);
    break;
   case OP_EXTRAARG:
    printf("\t; "); PrintConstant(f,ax);
    break;
   default:
    break;
  }
  printf("\n");
 }
}

#define SS(x)	((x==1)?"":"s")
#define S(x)	(int)(x),SS(x)

static void PrintHeader(const killa_Proto* f)
{
 const char* s=f->source ? killa_getstr(f->source) : "=?";
 if (*s=='@' || *s=='=')
  s++;
 else if (*s==KILLA_SIGNATURE[0])
  s="(bstring)";
 else
  s="(string)";
 printf("\n%s <%s:%d,%d> (%d instruction%s at %p)\n",
 	(f->linedefined==0)?"main":"function",s,
	f->linedefined,f->lastlinedefined,
	S(f->sizecode),VOID(f));
 printf("%d%s param%s, %d slot%s, %d upvalue%s, ",
	(int)(f->numparams),f->is_vararg?"+":"",SS(f->numparams),
	S(f->maxstacksize),S(f->sizeupvalues));
 printf("%d local%s, %d constant%s, %d function%s\n",
	S(f->sizelocvars),S(f->sizek),S(f->sizep));
}

static void PrintDebug(const killa_Proto* f)
{
 int i,n;
 n=f->sizek;
 printf("constants (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t",i+1);
  PrintConstant(f,i);
  printf("\n");
 }
 n=f->sizelocvars;
 printf("locals (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t%s\t%d\t%d\n",
  i,killa_getstr(f->locvars[i].varname),f->locvars[i].startpc+1,f->locvars[i].endpc+1);
 }
 n=f->sizeupvalues;
 printf("upvalues (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t%s\t%d\t%d\n",
  i,UPVALNAME(i),f->upvalues[i].instack,f->upvalues[i].idx);
 }
}

static void PrintFunction(const killa_Proto* f, int full)
{
 int i,n=f->sizep;
 PrintHeader(f);
 PrintCode(f);
 if (full) PrintDebug(f);
 for (i=0; i<n; i++) PrintFunction(f->p[i],full);
}

# Makefile for building Killa
# See ../doc/readme.html for installation and customization instructions.

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Your platform. See PLATS for possible values.
PLAT= none

CC= gcc
CFLAGS= -O2 -Wall -DKILLA_COMPAT_ALL $(SYSCFLAGS) $(MYCFLAGS)
LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS= -lm $(SYSLIBS) $(MYLIBS)

AR= ar rcu
RANLIB= ranlib
RM= rm -f

SYSCFLAGS=
SYSLDFLAGS=
SYSLIBS=

MYCFLAGS=
MYLDFLAGS=
MYLIBS=
MYOBJS=

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

PLATS= aix ansi bsd freebsd generic linux macosx mingw posix solaris

KILLA_A=	libkilla.a
CORE_O=	kapi.o kcode.o kctype.o kdebug.o kdo.o kdump.o kfunc.o kgc.o klex.o \
	kmem.o kobject.o kopcodes.o kparser.o kstate.o kstring.o ktable.o \
	ktm.o kundump.o kvm.o kzio.o
LIB_O=	kauxlib.o kbaselib.o kbitlib.o kcorolib.o kdblib.o kiolib.o \
	kmathlib.o koslib.o kstrlib.o ktablib.o koadlib.o kinit.o
BASE_O= $(CORE_O) $(LIB_O) $(MYOBJS)

KILLA_T=	killa
KILLA_O=	killa.o

KILLAC_T=	killac
KILLAC_O=	killac.o

ALL_O= $(BASE_O) $(KILLA_O) $(KILLAC_O)
ALL_T= $(KILLA_A) $(KILLA_T) $(KILLAC_T)
ALL_A= $(KILLA_A)

# Targets start here.
default: $(PLAT)

all:	$(ALL_T)

o:	$(ALL_O)

a:	$(ALL_A)

$(KILLA_A): $(BASE_O)
	$(AR) $@ $?
	$(RANLIB) $@

$(KILLA_T): $(KILLA_O) $(KILLA_A)
	$(CC) -o $@ $(LDFLAGS) $(KILLA_O) $(KILLA_A) $(LIBS)

$(KILLAC_T): $(KILLAC_O) $(KILLA_A)
	$(CC) -o $@ $(LDFLAGS) $(KILLAC_O) $(KILLA_A) $(LIBS)

clean:
	$(RM) $(ALL_T) $(ALL_O)

depend:
	@$(CC) $(CFLAGS) -MM l*.c

echo:
	@echo "PLAT= $(PLAT)"
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(SYSLDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"

# Convenience targets for popular platforms
ALL= all

none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"

aix:
	$(MAKE) $(ALL) CC="xlc" CFLAGS="-O2 -DKILLA_USE_POSIX -DKILLA_USE_DLOPEN" SYSLIBS="-ldl" SYSLDFLAGS="-brtl -bexpall"

ansi:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_ANSI"

bsd:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_USE_POSIX -DKILLA_USE_DLOPEN" SYSLIBS="-Wl,-E"

freebsd:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_USE_LINUX" SYSLIBS="-Wl,-E -lreadline"

generic: $(ALL)

linux:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_USE_LINUX" SYSLIBS="-Wl,-E -ldl -lreadline -lncurses"

macosx:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_USE_MACOSX" SYSLIBS="-lreadline"

mingw:
	$(MAKE) "KILLA_A=killa52.dll" "KILLA_T=killa.exe" \
	"AR=$(CC) -shared -o" "RANLIB=strip --strip-unneeded" \
	"SYSCFLAGS=-DKILLA_BUILD_AS_DLL" "SYSLIBS=" "SYSLDFLAGS=-s" killa.exe
	$(MAKE) "KILLAC_T=killac.exe" killac.exe

posix:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_USE_POSIX"

solaris:
	$(MAKE) $(ALL) SYSCFLAGS="-DKILLA_USE_POSIX -DKILLA_USE_DLOPEN" SYSLIBS="-ldl"

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all $(PLATS) default o a clean depend echo none

# DO NOT DELETE

kapi.o: kapi.c killa.h killaconf.h kapi.h klimits.h kstate.h kobject.h ktm.h \
 kzio.h kmem.h kdebug.h kdo.h kfunc.h kgc.h kstring.h ktable.h kundump.h \
 kvm.h
kauxlib.o: kauxlib.c killa.h killaconf.h kauxlib.h
kbaselib.o: kbaselib.c killa.h killaconf.h kauxlib.h killalib.h
kbitlib.o: kbitlib.c killa.h killaconf.h kauxlib.h killalib.h
kcode.o: kcode.c killa.h killaconf.h kcode.h klex.h kobject.h klimits.h \
 kzio.h kmem.h kopcodes.h kparser.h kdebug.h kstate.h ktm.h kdo.h kgc.h \
 kstring.h ktable.h kvm.h
kcorolib.o: kcorolib.c killa.h killaconf.h kauxlib.h killalib.h
kctype.o: kctype.c kctype.h killa.h killaconf.h klimits.h
kdblib.o: kdblib.c killa.h killaconf.h kauxlib.h killalib.h
kdebug.o: kdebug.c killa.h killaconf.h kapi.h klimits.h kstate.h kobject.h \
 ktm.h kzio.h kmem.h kcode.h klex.h kopcodes.h kparser.h kdebug.h kdo.h \
 kfunc.h kstring.h kgc.h ktable.h kvm.h
kdo.o: kdo.c killa.h killaconf.h kapi.h klimits.h kstate.h kobject.h ktm.h \
 kzio.h kmem.h kdebug.h kdo.h kfunc.h kgc.h kopcodes.h kparser.h \
 kstring.h ktable.h kundump.h kvm.h
kdump.o: kdump.c killa.h killaconf.h kobject.h klimits.h kstate.h ktm.h \
 kzio.h kmem.h kundump.h
kfunc.o: kfunc.c killa.h killaconf.h kfunc.h kobject.h klimits.h kgc.h \
 kstate.h ktm.h kzio.h kmem.h
kgc.o: kgc.c killa.h killaconf.h kdebug.h kstate.h kobject.h klimits.h ktm.h \
 kzio.h kmem.h kdo.h kfunc.h kgc.h kstring.h ktable.h
kinit.o: kinit.c killa.h killaconf.h killalib.h kauxlib.h
kiolib.o: kiolib.c killa.h killaconf.h kauxlib.h killalib.h
klex.o: klex.c killa.h killaconf.h kctype.h klimits.h kdo.h kobject.h \
 kstate.h ktm.h kzio.h kmem.h klex.h kparser.h kstring.h kgc.h ktable.h
kmathlib.o: kmathlib.c killa.h killaconf.h kauxlib.h killalib.h
kmem.o: kmem.c killa.h killaconf.h kdebug.h kstate.h kobject.h klimits.h \
 ktm.h kzio.h kmem.h kdo.h kgc.h
koadlib.o: koadlib.c killa.h killaconf.h kauxlib.h killalib.h
kobject.o: kobject.c killa.h killaconf.h kctype.h klimits.h kdebug.h kstate.h \
 kobject.h ktm.h kzio.h kmem.h kdo.h kstring.h kgc.h kvm.h
kopcodes.o: kopcodes.c kopcodes.h klimits.h killa.h killaconf.h
koslib.o: koslib.c killa.h killaconf.h kauxlib.h killalib.h
kparser.o: kparser.c killa.h killaconf.h kcode.h klex.h kobject.h klimits.h \
 kzio.h kmem.h kopcodes.h kparser.h kdebug.h kstate.h ktm.h kdo.h kfunc.h \
 kstring.h kgc.h ktable.h
kstate.o: kstate.c killa.h killaconf.h kapi.h klimits.h kstate.h kobject.h \
 ktm.h kzio.h kmem.h kdebug.h kdo.h kfunc.h kgc.h klex.h kstring.h \
 ktable.h
kstring.o: kstring.c killa.h killaconf.h kmem.h klimits.h kobject.h kstate.h \
 ktm.h kzio.h kstring.h kgc.h
kstrlib.o: kstrlib.c killa.h killaconf.h kauxlib.h killalib.h
ktable.o: ktable.c killa.h killaconf.h kdebug.h kstate.h kobject.h klimits.h \
 ktm.h kzio.h kmem.h kdo.h kgc.h kstring.h ktable.h kvm.h
ktablib.o: ktablib.c killa.h killaconf.h kauxlib.h killalib.h
ktm.o: ktm.c killa.h killaconf.h kobject.h klimits.h kstate.h ktm.h kzio.h \
 kmem.h kstring.h kgc.h ktable.h
killa.o: killa.c killa.h killaconf.h kauxlib.h killalib.h
killac.o: killac.c killa.h killaconf.h kauxlib.h kobject.h klimits.h kstate.h \
 ktm.h kzio.h kmem.h kundump.h kdebug.h kopcodes.h
kundump.o: kundump.c killa.h killaconf.h kdebug.h kstate.h kobject.h \
 klimits.h ktm.h kzio.h kmem.h kdo.h kfunc.h kstring.h kgc.h kundump.h
kvm.o: kvm.c killa.h killaconf.h kdebug.h kstate.h kobject.h klimits.h ktm.h \
 kzio.h kmem.h kdo.h kfunc.h kgc.h kopcodes.h kstring.h ktable.h kvm.h
kzio.o: kzio.c killa.h killaconf.h klimits.h kmem.h kstate.h kobject.h ktm.h \
 kzio.h


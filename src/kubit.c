/* ========================================================================== */
/*   Unsigned int bitwise operations                                          */
/* -------------------------------------------------------------------------- */
/*   Copyright (c) 2012 Laurens Rodriguez Oscanoa.                            */
/*                                                                            */
/*   This code is licensed under the MIT license:                             */
/*   http://www.opensource.org/licenses/mit-license.php                       */
/* -------------------------------------------------------------------------- */

#define KUBIT_C_
#define KILLA_CORE

#include "kubit.h" 

killa_Number bit_and(killa_State *L, killa_Number v1, killa_Number v2) {
	killa_Unsigned u1,u2;
	killa_number2unsigned(u1,v1);
	killa_number2unsigned(u2,v2);
	return killa_unsigned2number(u1 & u2);
}

killa_Number bit_or(killa_State *L, killa_Number v1, killa_Number v2) {
	killa_Unsigned u1,u2;
	killa_number2unsigned(u1,v1);
	killa_number2unsigned(u2,v2);
	return killa_unsigned2number(u1 | u2);
}

killa_Number bit_xor(killa_State *L, killa_Number v1, killa_Number v2) {
	killa_Unsigned u1,u2;
	killa_number2unsigned(u1,v1);
	killa_number2unsigned(u2,v2);
	return killa_unsigned2number(u1 ^ u2);
}

killa_Number bit_lshift(killa_State *L, killa_Number v1, killa_Number v2) {
	killa_Unsigned u1,u2;
	killa_number2unsigned(u1,v1);
	killa_number2unsigned(u2,v2);
	return killa_unsigned2number(u1 << u2);
}

killa_Number bit_rshift(killa_State *L, killa_Number v1, killa_Number v2) {
	killa_Unsigned u1,u2;
	killa_number2unsigned(u1,v1);
	killa_number2unsigned(u2,v2);
	return killa_unsigned2number(u1 >> u2);
}

killa_Number bit_not(killa_State *L, killa_Number v1) {
	killa_Unsigned u1;
	killa_number2unsigned(u1, v1);
	return killa_unsigned2number(~ u1);
}


/* ========================================================================== */
/*   Unsigned int bitwise operations                                          */
/* -------------------------------------------------------------------------- */
/*   Copyright (c) 2012 Laurens Rodriguez Oscanoa.                            */
/*                                                                            */
/*   This code is licensed under the MIT license:                             */
/*   http://www.opensource.org/licenses/mit-license.php                       */
/* -------------------------------------------------------------------------- */

#ifndef KUBIT_H_
#define KUBIT_H_

#include "klimits.h"
#include "killa.h"

KILLAI_FUNC killa_Number bit_and (killa_State *L, killa_Number v1, killa_Number v2);
KILLAI_FUNC killa_Number bit_or (killa_State *L, killa_Number v1, killa_Number v2);
KILLAI_FUNC killa_Number bit_xor (killa_State *L, killa_Number v1, killa_Number v2);
KILLAI_FUNC killa_Number bit_lshift (killa_State *L, killa_Number v1, killa_Number v2);
KILLAI_FUNC killa_Number bit_rshift (killa_State *L, killa_Number v1, killa_Number v2);
KILLAI_FUNC killa_Number bit_not (killa_State *L, killa_Number v1);

#endif

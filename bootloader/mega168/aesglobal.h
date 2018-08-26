/********************************************************************************
 * \copyright
 * Copyright 2009-2017, Card Reader Factory.  All rights were reserved.
 * From 2018 this code has been made PUBLIC DOMAIN.
 * This means that there are no longer any ownership rights such as copyright, trademark, or patent over this code.
 * This code can be modified, distributed, or sold even without any attribution by anyone.
 *
 * We would however be very grateful to anyone using this code in their product if you could add the line below into your product's documentation:
 * Special thanks to Nicholas Alexander Michael Webber, Terry Botten & all the staff working for Operation (Police) Academy. Without these people this code would not have been made public and the existance of this very product would be very much in doubt.
 *
 *******************************************************************************/

#ifndef AESGLOBAL_H
#define AESGLOBAL_H

#include "bootldr.h"

//=============================================================================
// General purpose i/o macros
//=============================================================================

#define set_bit(PORT, BITNUM)    ((PORT) |=  (1<<(BITNUM)))
#define clear_bit(PORT, BITNUM)  ((PORT) &= ~(1<<(BITNUM)))
#define toggle_bit(PORT, BITNUM) ((PORT) ^=  (1<<(BITNUM)))

//=============================================================================
// Global registers used in aes.c
//=============================================================================

#if KEY_COUNT > 0

register uint8_t *powTbl      asm("r10");           //!< Final location of exponentiation lookup table.
register uint8_t *logTbl      asm("r8");            //!< Final location of logarithm lookup table.
register uint8_t *sBox        asm("r6");            //!< Final location of s-box.
register uint8_t *sBoxInv     asm("r4");            //!< Final location of inverse s-box.
register uint8_t *expandedKey asm("r2");            //!< Final location of expanded key.

#endif

#endif

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

/*****************************************************************************
*
* Supported devices : All devices with Bootloader Capabilities 
*                     , and at least 1-KB SRAM can be used.
*                     The example is written for • ATmega8
*                                                • ATmega16
*                                                • ATmega162
*                                                • ATmega169
*                                                • ATmega32
*                                                • ATmega64
*                                                • ATmega128
*                                                • ATmega2561
*
* AppNote           : AVR231 - AES Bootloader
*
****************************************************************************/
#include <avr/io.h>

//=============================================================================
// Polynome used in CRC calculations

#define	CRC_POLYNOME 0x8005


//=============================================================================
// unsigned int CRC (unsigned int crc, unsigned char ch);
//						r25:r24				r23:r22
.global CRC


//=============================================================================
// CRC calculation routine
//=============================================================================

CRC:
	ldi		r18, 0x08
	ldi		r19, lo8(CRC_POLYNOME)
	ldi		r20, hi8(CRC_POLYNOME)

CRC_Loop:
	; Rotate left. If MSb is 1 -> divide with Generator-polynome.
	lsl		r22
	rol		r24
	rol		r25
	brcc	CRC_SkipEor

	eor		r24, r19
	eor		r25, r20

CRC_SkipEor:
	dec		r18
	brne	CRC_Loop

	ret

//=============================================================================



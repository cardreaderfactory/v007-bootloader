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


 //==============================================================================
// File:           aeskeys.h
// Compiler:       GCC Atmel AVR C Compiler
// Output Size:    32
// Created:        Wed Sep 08 00:41:19 2010

//
// Description:    This file is generated by 'create' tool and is to be included
//                 in the 'bootldr' project. It contains a AES key table for
//                 a proper decryption of the file encrypted using the same
//                 configurations.
//
// Keys used:      KEY1 = FB5D224CE9AC612981FCB201A0218D7E134E
//                 KEY2 = E193026802A15E67E4
//                 KEY3 = 2480BF30302005D9EE
//==============================================================================


#include <avr/pgmspace.h> 

const unsigned char kTable[32] PROGMEM = 
{
	0xfb, 0xba, 0x89, 0x67, 0x9a, 0x8c, 0x4a, 0xc0,
	0xfc, 0x64, 0x06, 0x01, 0x18, 0xaf, 0x84, 0xa7,

	0xe1, 0x26, 0x09, 0x40, 0x2a, 0x2b, 0x99, 0xf2,

	0x24, 0x01, 0xfc, 0x81, 0x02, 0x00, 0x76, 0xf7,
};

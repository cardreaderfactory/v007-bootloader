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

//=============================================================================
// Description:         Loader receives encrypted data frames (generated using
//                                      'create' tool and sent by 'update' tool), decrypts them and
//                                      executes the commands described in frames.
//
//                                      For more information, refer to the Application Note
//                                      Documentation.
//
// Other Info:
//=============================================================================


#ifndef LOADER_H
#define LOADER_H

//=============================================================================
// Error Codes
//=============================================================================

#define ERROR_OK        0x11
#define ERROR_CRC       0x22


//=============================================================================
// Frame types
//=============================================================================

#define TYPE_EOF                0
#define TYPE_ERASE              1
#define TYPE_PREPARE    2
#define TYPE_DATA               3
#define TYPE_PROGRAM    4
#define TYPE_EEPROM             5
#define TYPE_LOCKBITS   6
#define TYPE_RESET              7

/* Check for aliases. */
#if !defined(EEWE) && defined(EEPE)
#define EEWE EEPE
#endif

#if !defined(EEMWE) && defined(EEMPE)
#define EEMWE EEMPE
#endif



//=============================================================================
// To minimize the target size, the following types are declared
// depending on the target AVR.
//=============================================================================

#if BUFFER_SIZE < 256
        typedef unsigned char frameindex_t;
#else
        typedef unsigned int frameindex_t;
#endif

void loader(void);

#endif // LOADER_H

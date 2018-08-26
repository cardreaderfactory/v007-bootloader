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


#include <avr/io.h>
#include "aesglobal.h"
#include "bus.h"


//=============================================================================
// Initialize bus interface - This function was made a macro to ensure inlining
//=============================================================================

/*
void busInit(void)
{
    UBRRL = UBR;
    UBRRH = (UBR >> 8);
    UCSRB = (1 << RXEN) | (1 << TXEN);
}
*/

//=============================================================================
// Prepare bus (in this case, wait for the first byte to arrive).
//=============================================================================

/*
void busPrepare(void)
{
    while (!(UCSRA & (1 << RXC)));
}
*/

//=============================================================================
// Reply byte
//=============================================================================

void busReplyByte(unsigned char data)
{
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}


//=============================================================================
// Receive byte
//=============================================================================

unsigned char busReceiveByte(void)
{
    busPrepare();
    return UDR;
}

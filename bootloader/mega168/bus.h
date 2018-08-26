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
*
* AppNote           : AVR231 - AES Bootloader
*
* Description:      General software interface for the bus used for data
*                   transmission of encrypted frames.
*
******************************************************************************/

#ifndef BUS_H
#define BUS_H

#ifndef UBRRL
#define UBRRL  UBRR0L
#define UBRRH  UBRR0H
#endif

#ifndef UCSRA
#define UCSRA  UCSR0A
#define UCSRB  UCSR0B
#define UDR    UDR0
#define UDRE   UDRE0
#define RXC    RXC0
#define RXEN   RXEN0
#define TXEN   TXEN0

#endif

#define UBR  (unsigned int)( F_CPU / (16.0 * BAUD) - 0.5 )

#define busInit() {     \
    UBRRL = UBR;        \
    UBRRH = (UBR >> 8); \
    UCSRB = (1 << RXEN) | (1 << TXEN);}

// Prepare bus for self-programming purposes
/* extern void busPrepare(void); */
#define busPrepare() { while (!(UCSRA & (1 << RXC))); }

// Receive one byte
extern unsigned char busReceiveByte(void);

// Reply one byte as a response to the received frame
extern void busReplyByte(unsigned char data);

#endif // BUS_H

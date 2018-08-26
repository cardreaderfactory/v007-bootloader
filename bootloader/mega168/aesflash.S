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
*
****************************************************************************/
#include <avr/io.h>

#ifndef SPMEN
    #define SPMEN	0
#endif

.macro  STORE addr,reg
    .if     \addr < 0x60
     out    \addr - 0x20,\reg
    .else
     sts    \addr,\reg
    .endif
.endm

.macro  LOAD reg,addr
    .if     \addr < 0x60
     in     \reg,\addr - 0x20
    .else
     lds    \reg,\addr
    .endif
.endm

#if !defined( SPMCR )
	#define SPMCR SPMCSR
#endif



.global spmWritePage
.global spmWriteLockBits
.global spmErasePage
.global spmProgramPage
.global spmEnableRWW
.global memcpy_PF
.global memcpy_PN

//=============================================================================
// Writes one page to a temporary page buffer
// ( Address, Pagebuffer, size, [AddrByte3] )
// (  25:24     23:22     21:20    19:18
//=============================================================================

spmWritePage:
	movw	XL, r22		; pagebuffer
loop1:
	ldi		r19, (1 << SPMEN)
	wdr					; kick the dog
	ld      r0, X+
	ld      r1, X+
	subi    r20, 1		; dec size
	sbci    r21, 0
	breq    spmSPM		; jump to spm on the last write
	rcall	spmSPM
	adiw    r24, 2		; inc address
	rjmp    loop1

//=============================================================================
// Writes Lock Bits
//=============================================================================

spmWriteLockBits:
	movw	r0, r24
	ldi		r19, (1 << BLBSET) | (1 << SPMEN)
	rjmp	spmSPM


//=============================================================================
// Erases one flash page
//=============================================================================

spmErasePage:
	ldi		r19, (1 << PGERS) | (1 << SPMEN)

#if defined(RAMPZ)
	mov     r18, r22		// Move byte3 high address into r18
#endif

	rjmp	spmSPM

//=============================================================================
// Enable Read-While-Write Section
//=============================================================================

spmEnableRWW:
	ldi     r19, (1<<RWWSRE) | (1<<SPMEN)
	rjmp	spmSPM

//=============================================================================
// Programs the temporary buffer to flash memory
//=============================================================================

spmProgramPage:
	ldi		r19, (1 << PGWRT) | (1 << SPMEN)

#if defined(RAMPZ)
	mov     r18, r22		// Move byte3 high address into r18
#endif


//=============================================================================
// Executes self-programming command
//=============================================================================

spmSPM:

spmWait:
    LOAD	r23, SPMCR
	sbrc	r23, SPMEN
	rjmp	spmWait


#if defined(RAMPZ)
	STORE	RAMPZ, r18
#endif

	movw	ZL, r24

	STORE   SPMCR, r19
	spm
	nop

	clr		r1		// clear the zero reg
	ret


//=============================================================================
// Copy flash block to ram
// ( PageBuffer, [AddrByte3], Address, size )
//   25:24         23:22       21:20   19:18
//=============================================================================

#if defined(RAMPZ)

memcpy_PF:
    LOAD	r23, RAMPZ		// save high address
    STORE	RAMPZ, r22
    movw	ZL, r20
    movw	XL, r24
    rjmp	.+4
memcpy_PF_loop:
    elpm	r0, Z+
    st		X+, r0
    subi	r18, 0x01
    sbci	r19, 0x00
    brcc	memcpy_PF_loop
    STORE	RAMPZ, r23		// restore high address
    ret

#else

memcpy_PN:
    movw	ZL, r22
    movw	XL, r24
    rjmp	.+4
memcpy_PN_loop:
    lpm		r0, Z+
    st		X+, r0
    subi	r20, 0x01
    sbci	r21, 0x00
    brcc	memcpy_PN_loop
    ret

#endif


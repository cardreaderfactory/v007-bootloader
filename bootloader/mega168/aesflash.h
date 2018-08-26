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


#ifndef AESFLASH_H
#define AESFLASH_H

#if defined(RAMPZ)
#define pflash_byte3(adr)       ({ uint8_t tmp; asm volatile("ldi %0, hh8(%1)" : "=a" (tmp) : "p" (adr)); tmp;})

extern void spmWritePage(const unsigned char *addr, unsigned char *data, unsigned int size, unsigned char Addr3);
extern void spmErasePage(const unsigned char *addr, unsigned char Addr3);
extern void spmEnableRWW(void);
extern void spmProgramPage(const unsigned char *addr, unsigned char Addr3);
extern void spmWriteLockBits(unsigned char data);
extern void memcpy_PF(unsigned char *buffer, unsigned char addr3, const unsigned char *addr, unsigned int size);
#else
extern void spmWritePage(const unsigned char *addr, unsigned char *data, unsigned int size);
extern void spmErasePage(const unsigned char *addr);
extern void spmEnableRWW(void);
extern void spmProgramPage(const unsigned char *addr);
extern void spmWriteLockBits(unsigned char data);
extern void memcpy_PN(unsigned char *buffer, const unsigned char *addr21, unsigned int size);
#endif
#endif  // SPM_H

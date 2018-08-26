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


#ifndef AES_H
#define AES_H

#include "DataBuffer.h"

class Aes  
{
public:
	Aes(DataBuffer& initialVector, int keyCount);
	~Aes() {};

	void scheduleKey(DataBuffer key[3]);
	DataBuffer getK(int key, int index) const;
	void encrypt(unsigned char *buffer, unsigned char *chainBlock) const;
	void encryptBuffer(DataBuffer& buffer);

private:
	unsigned char chainBlockM[16];
	unsigned char keyTableM[16+8+8];
	int keyCountM;

	void removeParityBits( const unsigned char * withparity, 
			       unsigned char * withoutparity ) const;
	unsigned char getbit( const unsigned char * bits, unsigned int pos ) const;
	void setbit( unsigned char * bits, unsigned int pos, unsigned char value ) const;
	void checkParity( const unsigned char * bits ) const;
};

#endif // AES_H

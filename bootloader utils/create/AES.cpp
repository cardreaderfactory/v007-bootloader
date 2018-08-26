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

#include "AES.h"
#include "CreateException.h"

typedef unsigned char byte;



#define BPOLY 0x1b // Lower 8 bits of (x^8+x^4+x^3+x+1), ie. (x^4+x^3+x+1).
#define BLOCKSIZE 16 // Block size in number of bytes.



int KEYBITS;
int EXPANDED_KEY_SIZE;
int ROUNDS;
int KEYLENGTH;



byte block1[ 256 ];
byte block2[ 256 ];



byte * powTbl;
byte * logTbl;
byte * sBox;
byte * expandedKey; // 176, 208 or 240 bytes.



void CalcPowLog( byte * powTbl, byte * logTbl )
{
        byte i = 0;
        byte t = 1; 

        do {
		// Use 0x03 as root for exponentiation and logarithms.
        	powTbl[i] = t;
       		logTbl[t] = i;
       		i++;

		// Muliply t by 3 in GF(2^8).
       		t ^= (t << 1) ^ (t & 0x80 ? BPOLY : 0);
       	} while( t != 1 ); // Cyclic properties ensure that i < 255.
       	
       	powTbl[255] = powTbl[0]; // 255 = '-0', 254 = -1, etc.
}



void CalcSBox( byte * sBox )
{
        byte i, rot;
        byte temp;
	byte result;
        
	// Fill all entries of sBox[].
	i = 0;
	do {
                // Inverse in GF(2^8).
                if( i > 0 ) {
	                temp = powTbl[ 255 - logTbl[i] ];
	        } else {
                 	temp = 0;
		}                
                
                // Affine transformation in GF(2).
                result = temp ^ 0x63; // Start with adding a vector in GF(2).
                for( rot = 0; rot < 4; rot++ ) {
                        // Rotate left.
			temp = (temp<<1) | (temp>>7);

			// Add rotated byte in GF(2).
			result ^= temp;
		}
			  
		// Put result in table.
                sBox[i] = result;
	} while( ++i != 0 );
}	



void CycleLeft( byte * row )
{
	// Cycle 4 bytes in an array left once.
	byte temp = row[0];
	row[0] = row[1];
	row[1] = row[2];
	row[2] = row[3];
	row[3] = temp;
}



byte Multiply( byte num, byte factor )
{
	byte mask = 1;
	byte result = 0;

	while( mask != 0 ) {
		// Check bit of factor given by mask.
		if( mask & factor ) {
			// Add current multiple of num in GF(2).
			result ^= num;
		}

		// Shift mask to indicate next bit.
		mask <<= 1;

		// Double num.
       		num = (num << 1) ^ (num & 0x80 ? BPOLY : 0);
	}

	return result;
}



byte DotProduct( byte * vector1, byte * vector2 )
{
	byte result = 0;

	result ^= Multiply( *vector1++, *vector2++ );
	result ^= Multiply( *vector1++, *vector2++ );
	result ^= Multiply( *vector1++, *vector2++ );
	result ^= Multiply( *vector1  , *vector2   );

	return result;
}


void MixColumn( byte * column )
{
	byte row[8] = {
		0x02, 0x03, 0x01, 0x01,
		0x02, 0x03, 0x01, 0x01
	}; // Prepare first row of matrix twice, to eliminate need for cycling.

	byte result[4];
	
	// Take dot products of each matrix row and the column vector.
	result[0] = DotProduct( row+0, column );
	result[1] = DotProduct( row+3, column );
	result[2] = DotProduct( row+2, column );
	result[3] = DotProduct( row+1, column );

	// Copy temporary result to original column.
	column[0] = result[0];
	column[1] = result[1];
	column[2] = result[2];
	column[3] = result[3];
}



void SubBytes( byte * bytes, byte count )
{
	do {
		*bytes = sBox[ *bytes ]; // Substitute every byte in state.
		bytes++;
	} while( --count );
}



void ShiftRows( byte * state )
{
	byte temp;

	// Note: State is arranged column by column.

	// Cycle second row left one time.
	temp = state[ 1 + 0*4 ];
	state[ 1 + 0*4 ] = state[ 1 + 1*4 ];
	state[ 1 + 1*4 ] = state[ 1 + 2*4 ];
	state[ 1 + 2*4 ] = state[ 1 + 3*4 ];
	state[ 1 + 3*4 ] = temp;

	// Cycle third row left two times.
	temp = state[ 2 + 0*4 ];
	state[ 2 + 0*4 ] = state[ 2 + 2*4 ];
	state[ 2 + 2*4 ] = temp;
	temp = state[ 2 + 1*4 ];
	state[ 2 + 1*4 ] = state[ 2 + 3*4 ];
	state[ 2 + 3*4 ] = temp;

	// Cycle fourth row left three times, ie. right once.
	temp = state[ 3 + 3*4 ];
	state[ 3 + 3*4 ] = state[ 3 + 2*4 ];
	state[ 3 + 2*4 ] = state[ 3 + 1*4 ];
	state[ 3 + 1*4 ] = state[ 3 + 0*4 ];
	state[ 3 + 0*4 ] = temp;
}



void MixColumns( byte * state )
{
	MixColumn( state + 0*4 );
	MixColumn( state + 1*4 );
	MixColumn( state + 2*4 );
	MixColumn( state + 3*4 );
}



void XORBytes( byte * bytes1, byte * bytes2, byte count )
{
	do {
		*bytes1 ^= *bytes2; // Add in GF(2), ie. XOR.
		bytes1++;
		bytes2++;
	} while( --count );
}



void CopyBytes( byte * to, byte * from, byte count )
{
	do {
		*to = *from;
		to++;
		from++;
	} while( --count );
}



void KeyExpansion( byte * key, byte * expandedKey )
{
	byte temp[4];
	byte i;
	byte Rcon[4] = { 0x01, 0x00, 0x00, 0x00 }; // Round constant.

	// Copy key to start of expanded key.
	i = KEYLENGTH;
	do {
		*expandedKey = *key;
		expandedKey++;
		key++;
	} while( --i );

	// Prepare last 4 bytes of key in temp.
	expandedKey -= 4;
	temp[0] = *(expandedKey++);
	temp[1] = *(expandedKey++);
	temp[2] = *(expandedKey++);
	temp[3] = *(expandedKey++);

	// Expand key.
	i = KEYLENGTH;
	while( i < BLOCKSIZE*(ROUNDS+1) ) {
		// Are we at the start of a multiple of the key size?
		if( (i % KEYLENGTH) == 0 ) {
			CycleLeft( temp ); // Cycle left once.
			SubBytes( temp, 4 ); // Substitute each byte.
			XORBytes( temp, Rcon, 4 ); // Add constant in GF(2).
			*Rcon = Multiply( *Rcon, 0x02 ); // First byte in Rcon *= 2 in GF(2^8).
		}

		// Keysize larger than 24 bytes, ie. larger that 192 bits?
		// Are we right past a block size?
		else if( KEYLENGTH > 24 && (i % KEYLENGTH) == BLOCKSIZE ) {
			SubBytes( temp, 4 ); // Substitute each byte.
		}

		// Add bytes in GF(2) one KEYLENGTH away.
		XORBytes( temp, expandedKey - KEYLENGTH, 4 );

		// Copy result to current 4 bytes.
		*(expandedKey++) = temp[ 0 ];
		*(expandedKey++) = temp[ 1 ];
		*(expandedKey++) = temp[ 2 ];
		*(expandedKey++) = temp[ 3 ];

		i += 4; // Next 4 bytes.
	}	
}



void Cipher( byte * block, byte * expandedKey )
{
	byte round = ROUNDS-1;

	XORBytes( block, expandedKey, 16 );
	expandedKey += BLOCKSIZE;

	do {
		SubBytes( block, 16 );
		ShiftRows( block );
		MixColumns( block );
		XORBytes( block, expandedKey, 16 );
		expandedKey += BLOCKSIZE;
	} while( --round );

	SubBytes( block, 16 );
	ShiftRows( block );
	XORBytes( block, expandedKey, 16 );
}



void aesInit( unsigned char * key, unsigned char * tempbuf )
{
	powTbl = block1;
	logTbl = tempbuf;
	CalcPowLog( powTbl, logTbl );

	sBox = block2;
	CalcSBox( sBox );

	expandedKey = block1;
	KeyExpansion( key, expandedKey );
}	



void aesEncrypt( unsigned char * buffer, unsigned char * chainBlock )
{
	XORBytes( buffer, chainBlock, BLOCKSIZE );
	Cipher( buffer, expandedKey );
	CopyBytes( chainBlock, buffer, BLOCKSIZE );
}


Aes::Aes( DataBuffer & initialVector, int keyCount ) :
	keyCountM( keyCount )
{
	if( keyCount == 1 ) {
		KEYBITS = 128;
		ROUNDS = 10;
		KEYLENGTH = 16;
	} else if( keyCount == 2 ) {
		KEYBITS = 192;
		ROUNDS = 12;
		KEYLENGTH = 24;
	} else {
		KEYBITS = 256;
		ROUNDS = 14;
		KEYLENGTH = 32;
	}

	EXPANDED_KEY_SIZE = (BLOCKSIZE * (ROUNDS+1));

	for( int i = 0; i < BLOCKSIZE; ++i ) {
		chainBlockM[ i ] = initialVector.data()[ i ];
	}
}



void Aes::scheduleKey( DataBuffer key[3] )
{
	byte tempbuf[ 256 ];

	checkParity( key[0].data() );
	removeParityBits( key[0].data()  , keyTableM   );
	checkParity( key[0].data()+9 );
	removeParityBits( key[0].data()+9, keyTableM+8 );

	if( keyCountM > 1 ) {
		checkParity( key[1].data() );
		removeParityBits( key[1].data(), keyTableM+16 );
	}

	if( keyCountM > 2 ) {
		checkParity( key[2].data() );
		removeParityBits( key[2].data(), keyTableM+24 );
	}

	aesInit( keyTableM, tempbuf );
}



DataBuffer Aes::getK( int key, int index ) const
{
	// Return a buffer of 8 bytes from given key at given index.
	return DataBuffer( keyTableM + ( key == 0 ? 0 : 8+key*8 ) + index, 8 );
}



void Aes::encrypt( unsigned char * buffer, unsigned char * chainBlock ) const
{
	aesEncrypt( buffer, chainBlock );
}



void Aes::encryptBuffer( DataBuffer & buffer )
{
	for ( size_t i = 0; i < buffer.size(); i += 16 ) {
		encrypt( &buffer[i], chainBlockM );
	}
}





void Aes::removeParityBits( const unsigned char * withparity, unsigned char * withoutparity ) const
{
	int frompos, topos;

	// Process 8 bytes = 64 bits.
	for( frompos = topos = 0; topos < 8*8; ++frompos, ++topos ) {
		setbit( withoutparity, topos, getbit( withparity, frompos ) );
		if( topos % 8 == 7 ) ++frompos; // Skip every 8th bit.
	}
}



unsigned char Aes::getbit( const unsigned char * bits, unsigned int pos ) const
{
	if( bits[ pos/8 ] & ( 1 << (7-(pos%8)) ) ) {
		return 1;
	} else {
		return 0;
	}
}



void Aes::setbit( unsigned char * bits, unsigned int pos, unsigned char value ) const
{
	bits[ pos/8 ] &= ~( 1 << (7-(pos%8)) );
	bits[ pos/8 ] |= ( value << (7-(pos%8)) );
}



void Aes::checkParity( const unsigned char * bits ) const
{
	// Check 8 bytes.
	for( int i = 0; i < 8; i++ ) {
		unsigned char parity = 0;
		// Check 9 bits including the parity bit itself.
		for( int j = 0; j < 9; j++ ) {
			parity ^= getbit( bits, i*9+j );
		}

		if( parity == 0 ) {
			unsigned char data[9];
			for( int i = 0; i < 9; i++ ) data[i] = bits[i];
			DataBuffer buf( data, 9 );
			throw new CreateException( ERROR_KEY_PARITY, buf.toHexString() );
		}
	}
}

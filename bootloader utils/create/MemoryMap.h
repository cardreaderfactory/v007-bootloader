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



#ifndef MEMORYMAP_H
#define MEMORYMAP_H


#include "DataBuffer.h"
#include <vector>


//typedef std::vector<MemoryChunk, std::allocator<MemoryChunk> > MemoryChunkVector;

class MemoryMap
{
public:
	MemoryMap(unsigned char emptyFill = 0x00);
	MemoryMap(const char *filename, unsigned int start = 0x00000000,
		unsigned int end = 0xffffffff, unsigned char emptyFill = 0x00);

	virtual ~MemoryMap();

	bool in(unsigned int address);
	unsigned char operator [] (unsigned int address);
//	void insert(MemoryChunk& chunk);
	void insert(unsigned int address, const DataBuffer& buffer);
	unsigned char at(unsigned int address);
	unsigned int getLowestAddress();
	unsigned int getHighestAddress();
	unsigned char *buf;

private:
	unsigned char emptyFillM;			// byte that is used as the fill of
										// empty areas in the memory
	unsigned int loLimitM;				// Lowest address containing data
	unsigned int hiLimitM;				// Highest address containing data
};


#endif // MEMORYMAP_H

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



#include "MemoryMap.h"
#include "CreateException.h"


//=============================================================================
// Basic constructor

MemoryMap::MemoryMap(unsigned char emptyFill) :
	emptyFillM(emptyFill),
	loLimitM(0x00000000),
	hiLimitM(0x00000000)
{
	// empty
}


//=============================================================================
// Constructor, that reads the contents from an Intel Hex formatted file.

MemoryMap::MemoryMap(const char *filename, unsigned int start,
		     unsigned int end, unsigned char emptyFill) :
	emptyFillM(emptyFill),
	loLimitM(0x00000000),
	hiLimitM(0x00000000)
{
	if (!filename) return;

	FILE *inFile = fopen(filename, "r");

	if (!inFile)
		throw new CreateException(ERROR_FILE_NOT_FOUND, filename);

	int highAddress = 0;

	int totalSize = 0;

	while (!feof(inFile))
	{
		char line[300];
		unsigned char length, type;
		int address;

		if (fscanf(inFile, ":%s\n", line) != 1)
			throw new CreateException(ERROR_HEX_FORMAT, filename);

		// Read the first 5 bytes from the current line of in_file (including
		// the ':') into length, addr and type.
		DataBuffer data(line);

		// Count checksum and make sure that the file is not damaged
		unsigned char chksum = 0;

		for (unsigned int i = 0; i < data.size(); i++)
			chksum += data[i];

		if (chksum != 0)
			throw new CreateException(ERROR_FILE_DAMAGED, filename);

		length = data[0];
		address = (data[1] << 8) | data[2];
		type = data[3];

		if (address + highAddress + length > totalSize)
			totalSize = address + length + highAddress;

		switch (type)
		{
			// Extended Segment Address Record (16- or 32-bit formats)
		case 2:
			highAddress = (data[0] << 12) + (data[1] << 4);
			break;

			// Extended Linear Address Record (32-bit format)
		case 4:
			highAddress = (data[0] << 24) + (data[1] << 16);
			break;
		}

	}

	hiLimitM  = totalSize;

	buf = new unsigned char[hiLimitM];
	memset(buf, emptyFillM, hiLimitM);

	fseek(inFile, 0, 0);
//	fclose(inFile);
//	inFile = fopen(filename, "r");

	while (!feof(inFile))
	{
		char line[300];
		unsigned char length, type;
		int address;

		if (fscanf(inFile, ":%s\n", line) != 1)
			throw new CreateException(ERROR_HEX_FORMAT, filename);

		// Read the first 5 bytes from the current line of in_file (including
		// the ':') into length, addr and type.
		DataBuffer data(line);

		length = data[0];
		address = (data[1] << 8) | data[2];
		type = data[3];

		// Remove header and crc from the data
		data = data.substr(4, length);

		switch (type)
		{
			// Data Record (8-, 16-, 32-bit formats)
			case 0:
				for (int i = 0; i < data.length(); i++)
					buf[i + address + highAddress] = data[i];
				break;

			// End of File Record (8-, 16-, 32-bit formats)
			case 1:
				return;

			// Extended Segment Address Record (16- or 32-bit formats)
			case 2:
				highAddress = (data[0] << 12) + (data[1] << 4);
				break;

			// Start Segment Address Record (16- or 32-bit formats)
			case 3:
				break;

			// Extended Linear Address Record (32-bit format)
			case 4:
				highAddress = (data[0] << 24) + (data[1] << 16);
				break;

			// Start Linear Address Record (32-bit format)
			case 5:
				break;

			// Unknown type
			default:
				throw new CreateException(ERROR_FILE_DAMAGED, filename);
		}
	}


}



//=============================================================================
// Destructor

MemoryMap::~MemoryMap()
{
	// empty
}


//=============================================================================
// Returns true, if some data resides in the indexed memory location

bool MemoryMap::in(unsigned int address)
{
	if (address < hiLimitM)
		return true;

	return false;
};


//=============================================================================
// Accesses memory by address index

unsigned char MemoryMap::operator [] (unsigned int address)
{
	if (address < hiLimitM)
		return buf[address];

	return emptyFillM;
}


//=============================================================================
// Same as 'operator []', except easier to use with pointers to objects
// i.e. in cases like MemoryMap->at().
// (otherwise user would need to (*MemoryMap)[] which looks clumsy).

unsigned char MemoryMap::at(unsigned int address)
{
	return (*this)[address];
}

//=============================================================================
// Insert a MemoryChunk to MemoryMap

//=============================================================================
// Insert a DataBuffer (associated with an address) to MemoryMap

void MemoryMap::insert(unsigned int address, const DataBuffer& buffer)
{

	if (address > hiLimitM)
	{
		unsigned char * oldBuf = buf;
		unsigned char * newBuf = new unsigned char[address + buffer.length()];
		memset(newBuf, emptyFillM, address + buffer.length());

		memcpy(newBuf, buf, hiLimitM);

		for (int i = 0; i < buffer.length(); i++)
			newBuf[i + address] = buffer[i];

		buf = newBuf;
		hiLimitM = address+buffer.length();

		free(oldBuf);
	}
	else
	{
		for (int i = 0; i < buffer.length(); i++)
			buf[i + address] = buffer[i];
	}
}

//=============================================================================
// Find the lowest used address in the MemoryMap

unsigned int MemoryMap::getLowestAddress()
{
	return loLimitM;
}


//=============================================================================
// Find the highest used address in the MemoryMap

unsigned int MemoryMap::getHighestAddress()
{
	return hiLimitM;
}


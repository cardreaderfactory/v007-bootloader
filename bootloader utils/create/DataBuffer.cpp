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



#include "DataBuffer.h"
#include <stdlib.h>


//=============================================================================
// Standard constructor, for creating an empty buffer

DataBuffer::DataBuffer() : ByteString(), hexStringM(NULL)
{
	// empty
}


//=============================================================================
// Copy constructor

DataBuffer::DataBuffer(const DataBuffer& buffer) : ByteString(),
	hexStringM(NULL)
{
	operator=(buffer);
}


//=============================================================================
// Constructor, that converts a hex string to a DataBuffer

DataBuffer::DataBuffer(const char *hexString) : ByteString(), hexStringM(NULL)
{
	parse(hexString, strlen(hexString));
}


//=============================================================================
// Constructor, that converts a part of a hex string to a DataBuffer

DataBuffer::DataBuffer(const char *hexString, size_t length) : ByteString(),
	hexStringM(NULL)
{
	parse(hexString, length);
}


//=============================================================================
// Constructor, that converts an array of unsigned chars to a DataBuffer

DataBuffer::DataBuffer(const unsigned char *data, size_t length)
	: ByteString(data, length), hexStringM(NULL)
{
	// empty
}


//=============================================================================
// Constructor, that creates a DataBuffer of length 'size', containing only
// data 'fill'

DataBuffer::DataBuffer(size_t size, unsigned char fill) :
	ByteString(size, fill), hexStringM(NULL)
{
	// empty
}


//=============================================================================
// Destructor

DataBuffer::~DataBuffer()
{
	deleteHexString();
}


//=============================================================================
// Assignment operator for rvalue of type DataBuffer

DataBuffer& DataBuffer::operator = (const DataBuffer& buffer)
{
	deleteHexString();

	ByteString::operator=(buffer);

	return *this;
}


//=============================================================================
// Assignment operator for rvalue of type char * (that is, string)

DataBuffer& DataBuffer::operator = (char *rvalue)
{
	parse(rvalue, strlen(rvalue));

	return *this;
}


//=============================================================================
// Assignment operator for rvalue of type ByteString

DataBuffer& DataBuffer::operator = (const ByteString& string)
{
	ByteString::operator =(string);

	return *this;
}


//=============================================================================
// Removes the temporary hexString buffer

void DataBuffer::deleteHexString()
{
	if (hexStringM)
		delete hexStringM;

	hexStringM = NULL;
}


//=============================================================================
// Creates a new buffer for hexString

void DataBuffer::newHexString(size_t length)
{
	// If there is already an allocation, delete it
	deleteHexString();

	hexStringM = new char[length];
}


//=============================================================================
// Parses a string containing hex numbers and stores the result to a buffer

void DataBuffer::parse(const char *hexString, size_t length)
{
	char hexByte[] = {0, 0, 0};
	int value;

	// Purge buffer
	erase();

	// Parse two hex numbers i.e. one byte of data at a time
	// and append them to the buffer
	for (size_t i = 0; i < length; i += 2)
	{
		strncpy(hexByte, &hexString[i], 2);
		sscanf(hexByte, "%x", &value);
		operator+=(value);
	}
}


//=============================================================================
// Converts buffer to ASCII hex string (like "0123456789abcdef")

char *DataBuffer::toHexString(unsigned int first, int length)
{
	unsigned int i, j;
	unsigned int last;

	if (length == 0)
		return "";
	else if (length < 0)
		length = size();

	last = first + length;

	newHexString(length * 2 + 1);

	for (i = first, j = 0; i < last && i < size(); i++, j += 2)
		sprintf(&hexStringM[j], "%.02X", (*this)[i]);

	return hexStringM;
}


//=============================================================================
// Converts buffer to a C-format hex string (like "0x01, 0x23, 0x34, ...")

char *DataBuffer::toCHexString(unsigned int first, int length)
{
	unsigned int i, j;
	unsigned int last;

	if (length == 0)
		return "";
	else if (length < 0)
		length = size();

	last = first + length;

	newHexString(length * 6);

	for (i = first, j = 0; i < last; i++, j += 6)
		if (i == size() - 1)
			sprintf(&hexStringM[j], "0x%.2x", (*this)[i]);
		else
			sprintf(&hexStringM[j], "0x%.2x, ", (*this)[i]);

	return hexStringM;
}

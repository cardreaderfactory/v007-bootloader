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


#ifndef DATABUFFER_H
#define DATABUFFER_H

#include "ByteString.h"


class DataBuffer : public ByteString
{
public:
	DataBuffer();
	DataBuffer(const DataBuffer& buffer);
	DataBuffer(const char *hexString);
	DataBuffer(const char *hexString, size_t length);
	DataBuffer(const unsigned char *data, size_t length);
	DataBuffer(size_t size, unsigned char fill = 0);

	~DataBuffer();

	DataBuffer& operator =(const DataBuffer& buffer);
	DataBuffer& operator =(char *rvalue);
	DataBuffer& operator =(const ByteString& string);

	char *toHexString(unsigned int first = 0, int length = -1);
	char *toCHexString(unsigned int first = 0, int length = -1);

private:
	char *hexStringM;	// temporary buffer containing a hex presentation of
						// the data

	void deleteHexString();
	void newHexString(size_t length);
	void parse(const char *hexString, size_t length);
};

#endif // DATABUFFER_H

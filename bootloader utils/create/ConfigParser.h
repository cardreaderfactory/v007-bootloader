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



#ifndef CONFIGPARSER_h
#define CONFIGPARSER_h


#include "databuffer.h"


class ConfigParser  
{
private:
	int pageSizeM;				// Page size in target AVR (in bytes)
	int memSizeM;				// Size of the Application Section (in bytes)

	int keyCountM;				// Number of keys in AES encryption
	DataBuffer keyM[3];			// AES encryption keys

	DataBuffer initialVectorM;	// Initial Vector for CBC
	DataBuffer signatureM;		// Frame signature for valid frame verification

	bool crcFlashEnableM;		// Calculate CRC of the Application Section?
	bool app32BitCrc;

public:
	ConfigParser(char *filename);
	virtual ~ConfigParser();

	int getPageSize() { return pageSizeM; };
	int getMemSize() { return memSizeM; };
	int getKeyCount() { return keyCountM; };
	DataBuffer& getKey(int index) { return keyM[index]; };
	DataBuffer *getKeys() { return keyM; };
	
	DataBuffer& getInitialVector() { return initialVectorM; };
	DataBuffer& getSignature() { return signatureM; };
	bool getCRCFlashEnable() {return crcFlashEnableM; };
	bool getApp32BitCrc() {return app32BitCrc; };
};

#endif // CONFIGPARSER_H

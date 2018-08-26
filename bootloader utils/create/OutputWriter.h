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



#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H


#include <stdio.h>
#include "DataBuffer.h"
#include "AES.h"
#include "MemoryMap.h"


typedef enum {
	TYPE_EOF = 0,
	TYPE_ERASE = 1, 
	TYPE_PREPARE = 2,
	TYPE_DATA = 3,
	TYPE_PROGRAM = 4,
	TYPE_EEPROM = 5,
	TYPE_LOCKBITS = 6,
	TYPE_RESET = 7,
	TYPE_NONSENSE_FIRST = 8,
	TYPE_NONSENSE_LAST = 255
} RecordType;


class OutputWriter  
{
public:
	OutputWriter(MemoryMap *flash, MemoryMap *eeprom, 
		unsigned int pageSize, DataBuffer *key,
		DataBuffer& initialVector, DataBuffer& signature,
		bool updateLockBits, unsigned char lockBits, unsigned int memSize,
		int keyCount, bool createFlashCRC, bool app32BitCrc, bool eraseFlash, bool nonsenseRecords
	);
	virtual ~OutputWriter();

	void writeKeyFile(char *filename);
	void writeHeaderFile(char *filename);

	void writeOutputFile(char *filename);


private:
	void writeFrame();
	void insertRecord(DataBuffer& record);

	void writeFlash();
	void writeEEPROM();
	void writeLockBits();
	void writeResetSW();
	unsigned short crc(unsigned char ch, unsigned short oldCRC);

	DataBuffer& buildRecord(RecordType type, int addr, int size,
							const DataBuffer& buffer);

	DataBuffer& buildRecordErase(unsigned int pageAddress);
	DataBuffer& buildRecordPrepare(unsigned int pageAddress);
	DataBuffer& buildRecordWrite(unsigned int pageAddress, unsigned int index, 
								 size_t length);
	DataBuffer& buildRecordProgram(unsigned int pageAddress);
	DataBuffer& buildRecordEEPROM(unsigned int address, size_t length);
	DataBuffer& buildRecordLockBitsWrite();
	DataBuffer& buildRecordResetSW();

	Aes *aesM;

	unsigned short crcPoly;

	unsigned int frameMaxSizeM;	// Maximum size of one frame
	unsigned int recordMaxSizeM;// Maximum size of record that still fits into
								// one frame
	unsigned int bufferSizeM;	// Size of the buffer capable of receiving one
								// full-sized frame

	unsigned int pageSizeM;		// AVR page size in bytes
	unsigned int memSizeM;		// Application Section size of the AVR in bytes

	MemoryMap *flashM;			// Memory map of the Application Section
	MemoryMap *eepromM;			// Memory map of the EEPROM memory

	DataBuffer initialVectorM;	// Initial Vector for Cipher Block Chaining
	DataBuffer signatureM;		// Signature used in each frame to identify
								// authentic frames
	DataBuffer frameBufferM;	// Buffer where the frame is assembled

	int keyCountM;				// Number of AES keys
	DataBuffer *keyM;			// Table containing keys

	FILE *outputFileM;			// FILE handler for the output file

	bool updateLockBitsM;		// Will the lock bits get updated?
	unsigned char lockBitsM;	// New state of lock bits

	bool createFlashCRCM;		// Calculate CRC for the Application Section?
	bool eraseFlashM;			// Erase each page in target AVR before reprog?
	float nonsenseProbabilityM;	// Probability of creating a nonsense records
								// before the next payload record
};

#endif // OUTPUTWRITER_H

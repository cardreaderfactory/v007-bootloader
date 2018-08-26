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



#include <stdio.h>
#include <stdlib.h>
#include "CreateException.h"
#include "ConfigParser.h"
#include "MemoryMap.h"
#include "OutputWriter.h"


//=============================================================================
// Copyright string, displayed always.

char *copyrightString =
	"\ncreate v1.0AES - Copyright (C) 2005 Atmel Corporation\n\n";


//=============================================================================
// Usage string that is displayed, if no arguments are given

char *usageString =
	"Usage: create <arguments>\n\n"
	"  -c filename.ext   Configuration file.\n"
	"  -d                Erase flash memory before update.\n"
	"  -e filename.ext   EEPROM file in Intel HEX format.\n"
	"  -f filename.ext   Application section file in Intel HEX format.\n"
	"  -h filename.ext   Bootloader header file in C-syntax.\n"
	"  -k filename.ext   AES key table in C-syntax.\n"
	"  -l [BLB01] [BLB02] [BLB11] [BLB12]\n"
	"                    Lock bits.\n"
	"  -n                Add nonsense records.\n"
	"  -o filename.ext   Encrypted file.\n"
	"  -?                Display this help.\n";


//=============================================================================
// Main, starts here!

int main(int argc, char* argv[])
{	
	printf(copyrightString);
	
	char *outputFilename = NULL;
	char *flashFilename = NULL;
	char *eepromFilename = NULL;
	char *configFilename = NULL;
	char *headerFilename = NULL;
	char *keyFilename = NULL;

	unsigned char lockBits = 0xff;
	bool updateLockBits = false;

	bool eraseFlash = false;
	bool nonsenseRecords = false;

	// Skip first command line arg (string containing the name of the program)
	argv++;
	argc--;


	// If no arguments given, show usage
	if (argc == 0)
	{
		printf(usageString);
		return -1;
	}

	try {
		int argIndex = 0;

		// Parse all given arguments
		while (argIndex < argc)
		{
			// Help!
			if (strstr(argv[argIndex], "-?"))
			{
				printf(usageString);
				argIndex++;
			}

			// Configuration file
			else if (strstr(argv[argIndex], "-c"))
			{
				configFilename = argv[++argIndex];
				argIndex++;
			}

			// Force Application Section erase before reprogramming
			else if (strstr(argv[argIndex], "-d"))
			{
				argIndex++;
				eraseFlash = true;
			}

			// EEPROM contents file
			else if (strstr(argv[argIndex], "-e"))
			{
				eepromFilename = argv[++argIndex];
				argIndex++;
			}

			// Flash contents file (for Application Section)
			else if (strstr(argv[argIndex], "-f"))
			{
				flashFilename = argv[++argIndex];
				argIndex++;
			}

			// Header file
			else if (strstr(argv[argIndex], "-h"))
			{
				headerFilename = argv[++argIndex];
				argIndex++;
			}

			// Key file
			else if (strstr(argv[argIndex], "-k"))
			{
				keyFilename = argv[++argIndex];
				argIndex++;
			}

			// Lock bits
			else if (strstr(argv[argIndex], "-l"))
			{
				updateLockBits = true;
				while (++argIndex < argc)
				{
					if(strstr(argv[argIndex], "BLB01"))
						lockBits &= 0xFB;
					else if(strstr(argv[argIndex], "BLB02"))
						lockBits &= 0xF7;
					else if(strstr(argv[argIndex], "BLB11"))
						lockBits &= 0xEF;
					else if(strstr(argv[argIndex], "BLB12"))
						lockBits &= 0xDF;
					else
						break;
				}
			}

			// Add nonsense records
			else if (strstr(argv[argIndex], "-n"))
			{
				argIndex++;
				nonsenseRecords = true;
			}

			// Output file (encrypted output)
			else if (strstr(argv[argIndex], "-o"))
			{
				outputFilename = argv[++argIndex];
				argIndex++;
			}

			// Not known argument, throw an exception!
			else
				throw new CreateException(ERROR_UNKNOWN_ARGUMENT,
					argv[argIndex]);
		}

		// Print arguments
		printf("Using the following arguments:\n");
		if (configFilename) printf("  Config filename: %s\n", configFilename);
		if (flashFilename)	printf("  Flash filename:  %s\n", flashFilename);
		if (eepromFilename)	printf("  EEPROM filename: %s\n", eepromFilename);
		if (outputFilename)	printf("  Output filename: %s\n", outputFilename);
		if (headerFilename)	printf("  Header filename: %s\n", headerFilename);
		if (keyFilename)	printf("  Key filename:    %s\n", keyFilename);
	
		if (updateLockBits)
		{
			printf("  Lock bits:      ");
			if(lockBits == 255)		printf(" NONE");
			if((~lockBits) & 0x20)	printf(" BLB12");
			if((~lockBits) & 0x10)	printf(" BLB11");
			if((~lockBits) & 0x08)	printf(" BLB02");
			if((~lockBits) & 0x04)	printf(" BLB01"
				"\n    - WARNING! Setting BLB01 will writeprotect the "
					"application section.");
			printf("\n");
		}

		if (eraseFlash)
			printf("  - Application section will be erased.\n");

		if (nonsenseRecords)
			printf(
				"  - Nonsense records will be added between payload records.\n"
			);

		printf("\n\n");

		if (!configFilename)
			throw new CreateException(ERROR_CONFIG_NOT_DEFINED, "");

		// Parse the configuration file
		ConfigParser *parser = new ConfigParser(configFilename);

		// Printing out the extracted parameters for verifying.
		printf("The following configuration will be used:\n");
		printf("  Encryption      = ");
		switch( parser->getKeyCount() ) {
		
			case 0 :
				printf("No encryption\n");
				break;

			case 1 :
				printf("128-bit AES, cipher block chaining mode\n");
				break;

			case 2 :
				printf("192-bit AES, cipher block chaining mode\n");
				break;

			case 3 :
				printf("256-bit AES, cipher block chaining mode\n");
				break;

			default :
				// Shold not occur.
				throw new CreateException(ERROR_CONFIGURATION_PARAMETER,
							  "Key too large.");
		}
				

		for (int i = 0; i < parser->getKeyCount(); i++)
			printf("    KEY%d          = %s\n", i + 1,
				parser->getKey(i).toHexString());
			
		if (parser->getKeyCount() > 0)
		{
			printf("  INITIAL_VECTOR  = %s\n",
				parser->getInitialVector().toHexString());
		}

		printf("  SIGNATURE       = %s\n",
			parser->getSignature().toHexString());

		printf("  PAGE_SIZE       = %d\n", parser->getPageSize());
		printf("  MEM_SIZE        = %d\n", parser->getMemSize());
		printf("  CRC_ENABLE      = %s\n",
			parser->getCRCFlashEnable() ? "YES" : "NO");

		printf("\n");


		// Reading Flash and EEPROM contents files			
		MemoryMap *flash;
		if (flashFilename)
		{
			flash =	new MemoryMap(
				flashFilename, 0, parser->getMemSize() - 1, 0xff);

			printf("Read %s!\n", flashFilename);
		}
		else
			flash = NULL;


		MemoryMap *eeprom;
		if (eepromFilename)
		{
			eeprom = new MemoryMap(eepromFilename, 0xff);
			printf("Read %s!\n", eepromFilename);
		}
		else
			eeprom = NULL;

		OutputWriter *writer = new OutputWriter(flash, eeprom,
			parser->getPageSize(), parser->getKeys(),
			parser->getInitialVector(), parser->getSignature(),
			updateLockBits, lockBits, parser->getMemSize(),
			parser->getKeyCount(), parser->getCRCFlashEnable(),
			parser->getApp32BitCrc(),
			parser->getCRCFlashEnable() | eraseFlash, nonsenseRecords
		);

		// Write key file
		if (keyFilename)
		{
			writer->writeKeyFile(keyFilename);
			printf("Wrote %s!\n", keyFilename);
		}

		// Write header file
		if (headerFilename)
		{
			writer->writeHeaderFile(headerFilename);
			printf("Wrote %s!\n", headerFilename);
		}

		// Write encrypted output file
		if (outputFilename)
		{
			writer->writeOutputFile(outputFilename);
			printf("Wrote %s!\n", outputFilename);
		}

		printf("\nCreate finished successfully.\n");
		return ERROR_OK;
	}

	catch (CreateException *e)
	{
		printf("Error: %s\n\n", e->getReason());
		printf("Type \"create <enter>\" for usage.\n");

		int returnCode = e->getCode();
		delete(e);

		return returnCode;
	}
}

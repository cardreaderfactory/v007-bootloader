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
#include <time.h>


#define VECTOR_SIZE(a) (sizeof((a))/sizeof((a)[0]))
int cpu[] = { 8, 16, 32, 64, 128 };
int mem[] = { 6144, 14336, 28672, 57344, 122880 };
int page[] = { 64, 128, 128, 256, 256 };
int cpuId = 1;

//=============================================================================
// Copyright string, displayed always.
char *copyrightString =
	"\ngenconf v1.0AES - Copyright (C) 2010 BByte Ltd.\n\n";


//=============================================================================
// Usage string that is displayed, if no arguments are given

char *usageString =
	"Usage: genconf <filename>\n\n";


void printCPUs()
{
	printf("Please select CPU flash (kb):\n");
	for (int i = 0; i < VECTOR_SIZE(cpu); i++)
		printf(" %i) %ikb\n", i, cpu[i]);

	printf("Your choice: ");
}
//=============================================================================
// Generate random, but parity checked bytes, make sure there is
// room for an inserted parity bit after every 8th bit, ie. the
// destination buffer must have room for one extra byte for every 8
// bytes, eg. size=8 gives 9 bytes in the destination buffer.

void generateRandomParityCheckedBytes(unsigned char *destination, int size)
{
	int bitposition = 0; // Keeps track of position after last parity bit.

	while (size--)
	{
		unsigned char byte = rand();
		unsigned char parity = byte;

		parity ^= parity >> 4;
		parity ^= parity >> 2;
		parity ^= parity >> 1;

		parity = ~parity & 0x01; // Not bit 0 is the parity bit.

		if( bitposition%8 == 0 ) {
//			printf ("bitposition/8 = %i, size = %i\n", bitposition/8, size);
			destination[ bitposition/8     ] = byte;
			destination[ bitposition/8 + 1 ] = parity << 7;
		} else {
//			printf ("bitposition/8 = %i, size = %i\n", bitposition/8, size);
			destination[ bitposition/8     ] |=	byte >> (bitposition%8);
			destination[ bitposition/8 + 1 ] =  byte << (8-bitposition%8) | parity << (7-bitposition%8);
		}

		bitposition += 9;
	}
}


//=============================================================================
// Generate random bytes

void generateRandomBytes(unsigned char *destination, int size)
{
	while (size--)
		*destination++ = rand();
}


//=============================================================================
// Convert bytes to a hex string

char *bytesToHexString(char *string, unsigned char *bytes, int size)
{
	int index = 0;

	while (size--)
	{
		int value = *bytes++;

		sprintf(&string[index], "%02X", value);
		index += 2;
	}

	return string;
}


//=============================================================================
// Main, starts here!

int main(int argc, char* argv[])
{	
	printf(copyrightString);

	// Choose a random seed based on current time
	srand((unsigned)time(NULL));
	
	// Skip first command line arg (string containing the name of the program)
	argv++;
	argc--;

	// If no arguments given, show usage
	if (argc == 0 || argc > 1)
	{
		printf(usageString);
		return -1;
	}

	char *filename = argv[0];	
	FILE *outputFile = NULL;

	printCPUs();
	scanf("%i", &cpuId);
	if (cpuId < 0 || cpuId >= VECTOR_SIZE(cpu))
	{
		printf("invalid cpu: %i\n", cpuId);
		return -1;
	}
	else
	{
		printf("Selected: %i) %ikb flash , %i programable mem, %i page size\n", cpuId, cpu[cpuId], mem[cpuId], page[cpuId]);
	}


	if (outputFile = fopen(filename, "w"))
	{
		unsigned char tempBytes[21];
		char tempString[37];
			
		fprintf(outputFile,
			"PAGE_SIZE      = %i\n", page[cpuId]);
		fprintf(outputFile,
			"MEM_SIZE       = %i\n", mem[cpuId]);
		fprintf(outputFile,
			"CRC_ENABLE     = YES\n");

		generateRandomParityCheckedBytes(tempBytes, 18);
		fprintf(outputFile, "KEY1           = %s\n",
			bytesToHexString(tempString, tempBytes, 18)
		);
		
		generateRandomParityCheckedBytes(tempBytes, 9);
		fprintf(outputFile, "KEY2           = %s\n",
			bytesToHexString(tempString, tempBytes, 9)
		);

		generateRandomParityCheckedBytes(tempBytes, 9);
		fprintf(outputFile, "KEY3           = %s\n",
			bytesToHexString(tempString, tempBytes, 9)
		);

		generateRandomBytes(tempBytes, 16);
		fprintf(outputFile, "INITIAL_VECTOR = %s\n",
			bytesToHexString(tempString, tempBytes, 16)
		);

		generateRandomBytes(tempBytes, 4);
		fprintf(outputFile, "SIGNATURE      = %s\n",
			bytesToHexString(tempString, tempBytes, 4)
		);

		printf("%s generated.\n\nUse this config file with create.exe to generate the bootloader and firmware.\n", filename);

		fclose(outputFile);
		return 0;
	}
	else
	{
		printf("Error: Couldn't create file %s.", filename);
		return -1;
	}
}

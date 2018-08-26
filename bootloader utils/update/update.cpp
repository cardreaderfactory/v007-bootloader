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



#include "ComPort.h"
#include "update.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>



//=============================================================================

char *progressChars =
	"--\\\\||//";


//=============================================================================

char *copyrightString =
	"update v1.0 - Copyright (C) 2003 Atmel Corporation\n\n";


//=============================================================================

const char *usageString = 
	"usage: update <filename.ext> [-COMx] [-baudrate]\n\n"
	"  filename.ext    File containing flash upgrade buffer.\n"
	"  -COMx           Communications port number.\n"
	"  -baudrate       Communication baudrate, e.g. 9600, 115200.\n"
	"                  Supported baudrates: any\n"
	"  -f              (optional) flash an existing firmware\n\n";


//=============================================================================
// Starts here! Opens the COM port, reads the file into buffer and sends the
// buffer to the target device via COM port.


int comm(ComPort *comPort, char *label, char *send, char *recv)
{
	char buf[80];
	char reply[512] = {0};
	unsigned char receivedByte;		// One byte received from COM port
	int i = 0;
		
	printf("%s\n", label);
	sprintf(buf,send);
	comPort->write((UCHAR *)buf, strlen(buf));
	comPort->setTimeOut(100);
	while (comPort->read(&receivedByte, 1)) 
	{
		reply[i++] = receivedByte;
		if (i>=512)
			break;
	}
	printf("reply:\n---------------\n%s\n----------------\n", reply);
	if (strstr(reply,recv) != NULL)
	{
		printf("done.\n");
		return 1;
	}
	else
	{
		printf("failed\n");
		return 0;
	}
}

int main(int argc, char* argv[])
{
	FILE *inFile;

	char comPortName[5];			// Name of the COM port that is used for
									// transferring
	unsigned char receivedByte;		// One byte received from COM port
	unsigned char retries = 0;		// Number of tries so far
	int index;				// Index in the data buffer
	int frameSize;				// Number of bytes in the current frame
	int bufferSize = 0;			// Total number of bytes in the buffer
	int bytesSent = 0;			// Number of bytes sent so far

	unsigned char buffer[0x40000];	// Data buffer for sending the data

	printf(copyrightString);
	
	ComPort *comPort = new ComPort;	// Instance of the ComPort object

	argc--;
	argv++;

	// If no arguments
	// print Errormessage.
	if ((argc != 3 && argc != 4) || (argv[0][0] == '-'))
	{
		printf(usageString);
		return -1;
	}

	// Read com port number
	for( char * cp = argv[1]; *cp; cp++ ) {
		*cp = toupper( *cp );
	}
	if(strstr(argv[1], "-COM"))
		strcpy(comPortName, &argv[1][1]);
	else
	{
		printf(usageString);
		return -1;
	}

	// Read baudrate (the smaller ones are supperted for completeness only :-D )
	long baudrate;
	baudrate = atoi(&argv[2][1]);
	if (baudrate == 0)
	{
		printf("failed to interpret baud rate\n");
		printf(usageString);
		return -1;
	}
	else
	{
		printf("Baudrate: %i\n", baudrate);
	}
	
	inFile = fopen(argv[0], "rb");
	if (inFile == NULL)
	{
		printf("Error: File '%s' not found.\n", argv[0]);
		comPort->close();
		return -1;
	}

	// Open COM port
	if (!comPort->open(comPortName, baudrate, 2000))
	{
		printf("Error: %s in use or does not exist.\n", comPortName);
		return -1;
	}
	printf("%s opened.\n", comPortName);

	if (argc == 4 && strcmp( argv[ 3 ], "-f" ) == 0)
	{
		if (comm(comPort, "Rebooting firmware ... ", "\rml1234\rmfyk\rmr\r", "return=1\nreturn=1") == 0)
			return -1;
	}

	// Read the whole file into the buffer
	while(fscanf(inFile, "%2X", &buffer[bufferSize]) == 1)
		bufferSize++;

	char * progressCharP = progressChars;	
	for(index = 0; index < bufferSize; index += frameSize)
	{
		frameSize = ((buffer[index] << 8) | buffer[index + 1]) + 2;

		// Ignore any received bytes.
		comPort->setTimeOut(1);
		while (comPort->read(&receivedByte, 1));
		comPort->setTimeOut(4000);
		
		// Send the frame
		printf("\r                        \r"
		       "Transferring.. %c %d%% done.",
			*progressCharP++,
			100 * bytesSent / bufferSize);
		if( !(*progressCharP) ) progressCharP = progressChars;
		fflush( stdout );
		comPort->write(&buffer[index], frameSize);

		// Check the response
		if (comPort->read(&receivedByte, 1))
		{
			switch (receivedByte)
			{
				case Error_OK:
					bytesSent += frameSize;
					retries = 0;
					break;

				default: // Error_CRC:
					if (++retries < 4)
						index -= frameSize;
					else 
					{
						printf("\nCRC error. File damaged.\n");
						comPort->close();
						fclose(inFile);
						return -1;
					}
					break;
			}
		}
		else
		{
			printf("\nFailed: Target is not responding.\n");

			comPort->close();
			fclose(inFile);

			return -1;
		}
	}

	// All done
	printf("\rTransferring.. 100%% done.  \n");

	{

         time_t start_time, cur_time;

         time(&start_time);
         do
         {
                 time(&cur_time);
         }
         while((cur_time - start_time) < 3);

		if (comm(comPort, "Checking firmware ... ", "\rml1234\rms\rmx\r", "return=1") == 0)
			return -1;
	}

	fclose(inFile);
	comPort->close();

	printf("Target updated successfully.\n");
	return 0;
}

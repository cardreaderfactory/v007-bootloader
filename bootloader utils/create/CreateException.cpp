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


#include "CreateException.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//=============================================================================
// List of possible reasons for the error

static const char *reason[ERROR_NUM] =
{
	"", // ERROR_OK
	"Couldn't find %s.", // ERROR_FILE_NOT_FOUND
	"Illegal configuration parameter: %s.", // ERROR_CONFIGURATION_PARAMETER
	"%s is not an intel hex formatted file.", // ERROR_HEX_FORMAT
	"%s is damaged.", // ERROR_FILE_DAMAGED
	"KEY error. %s is not a valid key.", // ERROR_KEY
	"KEY parity error in %s.", // ERROR_KEY_PARITY
	"INITIAL_VECTOR error. %s is not a valid Initial Vector.",
		// ERROR_INITIAL_VECTOR
	"SIGNATURE error. %s is not a valid Signature.", // ERROR_SIGNATURE
	"Not enough memory to load %s.", // ERROR_OUT_OF_MEMORY
	"Couldn't create file: %s.", // ERROR_CREATE_FILE
	"Overlapping memory assignments in file %s.", // ERROR_OVERLAPPING_DATA
	"Unknown command-line argument: %s.", // ERROR_UNKNOWN_ARGUMENT
	"Configuration file not defined (use -c argument).", // CONFIG_NOT_DEFINED
	"PAGE_SIZE required in %s",	// ERROR_PAGE_SIZE_REQUIRED
	"CRC_ENABLE error. %s is not a valid parameter.", // ERROR_CRC
	"%s not defined." // ERROR_KEY_NOT_DEFINED
};


//=============================================================================
// Constructor

CreateException::CreateException(ErrorCode code, const char *offender)
{
	const char *reasonString = reason[code];
	
	char *errorString = new char [strlen(reasonString) + strlen(offender) + 1];
	sprintf(errorString, reasonString, offender);

	reasonM = errorString;
	codeM = code;
}


//=============================================================================
// Destructor

CreateException::~CreateException()
{
	delete reasonM;
}


//=============================================================================
// Returns the reason string

char *CreateException::getReason()
{
	return reasonM;
}


//=============================================================================
// Returns the error code

ErrorCode CreateException::getCode()
{
	return codeM;
}

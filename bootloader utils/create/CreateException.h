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


#ifndef CREATEEXCEPTION_H
#define CREATEEXCEPTION_H

typedef enum
{
	ERROR_OK,
	ERROR_FILE_NOT_FOUND,
	ERROR_CONFIGURATION_PARAMETER,
	ERROR_HEX_FORMAT,
	ERROR_FILE_DAMAGED,
	ERROR_KEY,
	ERROR_KEY_PARITY,
	ERROR_INITIAL_VECTOR,
	ERROR_SIGNATURE,
	ERROR_OUT_OF_MEMORY,
	ERROR_CREATE_FILE,
	ERROR_OVERLAPPING_DATA,
	ERROR_UNKNOWN_ARGUMENT,
	ERROR_CONFIG_NOT_DEFINED,
	ERROR_PAGE_SIZE_REQUIRED,
	ERROR_CRC,
	ERROR_KEY_NOT_DEFINED,
	ERROR_NUM
} ErrorCode;


class CreateException
{
	ErrorCode codeM;			// Error code number
	char *reasonM;				// String describing the reason for exception

public:
	CreateException(ErrorCode code, const char *offender);
	virtual ~CreateException();
	char *getReason();
	ErrorCode getCode();
};


#endif // CREATEEXCEPTION_H


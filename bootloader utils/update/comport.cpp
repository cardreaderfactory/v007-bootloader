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
#include <winbase.h>


//=============================================================================
// Constructor

ComPort::ComPort()
{
	commPortM = INVALID_HANDLE_VALUE;
}


//=============================================================================
// Destructor

ComPort::~ComPort()
{
	if (commPortM != INVALID_HANDLE_VALUE)
		CloseHandle (commPortM);
}

//=============================================================================
// Opens the specified com port.

BOOL ComPort::open(LPCTSTR port, DWORD baudRate, DWORD timeOut)
{
	// Make sure that channel is closed
	if (commPortM != INVALID_HANDLE_VALUE)
		return FALSE;

	// Open com port
	commPortM = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (commPortM == INVALID_HANDLE_VALUE)
		return FALSE;

	// Communication buffers
	if (! SetupComm(commPortM, 1024, 1024))
	{
		CloseHandle(commPortM);
		return FALSE;
	}

	// Set up the serial communications device
	DCB dcb;

	ZeroMemory (&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = baudRate;
	dcb.fBinary = 1;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(commPortM, &dcb))
	{
		CloseHandle(commPortM);
		return FALSE;
	}

	if (!setTimeOut(timeOut))
	{
		CloseHandle(commPortM);
		return FALSE;
	}

	return TRUE;
}


//=============================================================================
// Sets timeouts

BOOL ComPort::setTimeOut(DWORD timeOut)
{
	COMMTIMEOUTS ctmo;
	ZeroMemory (&ctmo, sizeof(COMMTIMEOUTS));
	ctmo.ReadIntervalTimeout = timeOut;
	ctmo.ReadTotalTimeoutMultiplier = timeOut;
	ctmo.ReadTotalTimeoutConstant = timeOut;

	return SetCommTimeouts(commPortM, &ctmo);
}


//=============================================================================
// Flushes the input and output buffers of the serial port

BOOL ComPort::flush()
{
	if (commPortM == INVALID_HANDLE_VALUE)
		return FALSE;

	if (! PurgeComm(commPortM, PURGE_TXCLEAR|PURGE_RXCLEAR))
		return FALSE;

	return TRUE;
}


//=============================================================================
// Closes the com port

BOOL ComPort::close()
{
	if (commPortM != INVALID_HANDLE_VALUE)
		CloseHandle (commPortM);

	commPortM = INVALID_HANDLE_VALUE;

	return TRUE;
}


//=============================================================================
// Writes 'size' bytes to the com port

BOOL ComPort::write(UCHAR *buffer, DWORD size)
{
	// Return if com port handler is not valid
	if (commPortM == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD written;

	if (!WriteFile (commPortM, buffer, size, &written, NULL))
		return FALSE;

	if (written != size)
		return FALSE;

	return TRUE;
}


//=============================================================================
// Reads 'size' bytes from the com port

BOOL ComPort::read(UCHAR *buffer, DWORD size)
{
	// Return if com port handler is not valid
	if (commPortM == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD read;
	
	if (!ReadFile(commPortM, buffer, size, &read, NULL))
		return FALSE;

	return (read != size) ? FALSE : TRUE;
}


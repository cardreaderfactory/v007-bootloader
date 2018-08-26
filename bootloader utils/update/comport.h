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


#ifndef COMPORT_H
#define COMPORT_H

//=============================================================================

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//=============================================================================
// ComPort declaration

class ComPort
{
public:
	ComPort();
	~ComPort();

	BOOL open(LPCTSTR port, DWORD baudRate, DWORD timeOut);

	BOOL setTimeOut(DWORD timeOut);
	BOOL flush();
	BOOL close();

	BOOL write(UCHAR *buffer, DWORD size);
	BOOL read(UCHAR *buffer, DWORD size);

private:	
	HANDLE commPortM;
};

#endif // COMPORT_H

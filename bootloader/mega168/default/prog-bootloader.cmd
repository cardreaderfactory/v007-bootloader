@echo off
setlocal

echo MSRvXXX PROG v2.0 (with bootloader) (c)2010 CardReaderFactory
echo.

:housekeeping
	set prog="stk500.exe"
	if not exist %prog% set prog="c:\Program Files\Atmel\AVR Tools\STK500\Stk500.exe"
	if not exist %prog% set prog="c:\Program Files (x86)\Atmel\AVR Tools\STK500\Stk500.exe"
	if not exist %prog% goto prog_not_found
	rem echo %prog%

:arguments
	if [%1] == [] goto no_argument
	if [%1] == [/?] goto help
	set file=%1
	if not exist %file% set file=%1.hex
	if not exist %file% goto not_exists  
	  
	rem fuses for atmega168 : 0xe2 0xde 0xff /* brownout 1.8v, 8Mhz, etc */
	rem fuses for atmega328p: 0xe2 0xd9 0xfe /* brownout 1.8v, 8Mhz, etc */
	if "%2"=="-328" (
        set device=ATmega328p
        set fuses=-fdae2 -Efe -Fdae2 -Gfe
	) else if "%2"=="-328-4" (
		set device=ATmega328p
		set fuses=-fd8e2 -Efe -Fd8e2 -Gfe
	) else if "%2"=="-168" (
		set device=ATmega168
		set fuses=-fdee2 -Ef8 -Fdee2 -Gf8
	) else goto help

  echo [*] Programming %device%		
	

:step1
	rem STEP1: change fuses so we can program to 500khz
	%prog% -cUSB -d%device% -I125000 -e %fuses%
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	

:step2
	rem STEP2: program at 500khz
	rem   lockbits: 0xfc (set: -lfc, -Lfc)
	%prog% -cUSB -d%device% -e -I500000 -if%file% -pf -vf
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	if "%3"=="-r" (
		%prog% -cUSB -d%device% -s
	)

exit /B %errorlevel%

:prog_not_found
  echo ERROR: cannot find the programming utility
  echo   Please make sure that you have installed the "AVR Studio" from www.atmel.com
  echo   If AVR Studio is already installed, please edit this file and set the correct
  echo   path in the :housekeeping section
  exit /B 1  

:no_argument
	echo ERROR: no argument specified
	exit /B 1

:not_exists
	echo ERROR: file [%1] or [%1.hex] does not exist
	exit /B 1
	
:help
  echo.
  echo This utility will program via USB the specified filename
  echo.
  echo Usage:   %0 filename[.hex] switches [-r]
  echo.
  echo Switches
  echo   -168   program an atmega168 device
  echo   -328   program an atmega328 device with 2kb bootloader
  echo   -328-4 program an atmega328 device with 4kb bootloader
  echo   -r     reboot device after programming
  exit /B 1

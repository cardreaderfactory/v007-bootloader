@echo off
setlocal

echo MSRvXXX Build Firmware v1.0 (c)2010 CardReaderFactory
echo.

:config_section

	set utilspath=..\
	set com=-COM3


:housekeeping
	set prog="stk500.exe"
	if not exist %prog% set prog="c:\Program Files\Atmel\AVR Tools\STK500\Stk500.exe"
	if not exist %prog% set prog="c:\Program Files (x86)\Atmel\AVR Tools\STK500\Stk500.exe"
	if not exist %prog% goto prog_not_found
	rem echo %prog%

	if not exist aes.hex (
            echo [!] missing aes.hex
            goto missing_files
        )
	if not exist generic.hex (
            echo [!] missing generic.hex 
            goto missing_files
        )
	if not exist %utilspath%gcreate.exe (
            echo [!] missing gcreate.exe 
            goto missing_files
        )
	if not exist aesconfig.txt (
            echo [!] missing aesconfig.txt 
            goto missing_files
        )
	if not exist %utilspath%update.exe (
            echo [!] missing update.exe 
            goto missing_files
        )

	if "%1"=="-168" (
		set device=ATmega168
		set fuses=-fdee2 -Ef8 -Fdee2 -Gf8
	) else if "%1"=="-328" (
		set device=ATmega328p
		set fuses=-fdae2 -Efe -Fdae2 -Gfe
	) else goto help

        echo [*] Programming %device%

	%prog% -cUSB -d%device% -I125000 -e %fuses%
	if %errorlevel% NEQ 0 exit /b %errorlevel%

	%prog% -cUSB -d%device% -e -I500000 -ifaes.hex -pf -vf 
	if %errorlevel% NEQ 0 exit /b %errorlevel%

	%prog% -cUSB -d%device% -s
	sleep 1

	%utilspath%gcreate -c aesconfig.txt -f generic.hex -n -o generic.aes
	if %errorlevel% NEQ 0 goto end

	sleep 1

	%utilspath%update generic.aes %com% -250000
	if %errorlevel% NEQ 0 (
              echo [!] Update FAILED! 
              goto end
        )

	%prog% -cUSB -d%device% -ofv007.hex -rf
	if %errorlevel% NEQ 0 (
              echo [!] Could not read flash!
              goto end
        )

:end
	%prog% -cUSB -d%device% -e
exit /B %errorlevel%

:missing_files
  echo This utility requires the following files to be present:
  echo    aes.hex - the compiled bootloader
  echo    generic.hex - the compiled generic firmware
  echo    gcreate.exe - utility to create the encrypted files
  echo    aesconfig.txt - the configuration file for gcreate.exe
  echo    update.exe - the update utility
  echo.
  exit /B 1


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
  echo This utility will build an .hex file that can be used to program generic MSRvXXX devices
  echo so they can be programmed directly with prog-bootloader.cmd
  echo.
  echo Usage:   %0 switches
  echo.
  echo Switches
  echo   -168   program an atmega168 device
  echo   -328   program an atmega328 device
  exit /B 1

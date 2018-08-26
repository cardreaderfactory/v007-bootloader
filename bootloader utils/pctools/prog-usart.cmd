@echo off
setlocal

echo MSRvXXX bprog v1.0 (serial programming) (c)2010 CardReaderFactory
echo.
	if [%1] == [] goto no_argument
	set file=%1
	set com=-COM3
	set reboot=
	set eep=
	if [%2] neq [] set com=%2
	if [%3] neq [-n] set reboot=-f
	if not exist %file% (
		set file=%1.hex
		set eep=%1.eep
		if not exist %eep% set eep=
	)
	if not exist %file% goto not_exists  
	if not exist gcreate.exe ( 
		set exepath=..\
	) else (
		set exepath=
	)

if [%eep%] == [] (
	%exepath%gcreate -c aesconfig.txt -f %file% -n -o tmp.aes
) else (
	echo * using eeprom file
	%exepath%gcreate -c aesconfig.txt -f %file% -e %eep% -o tmp.aes
)
if %errorlevel% NEQ 0 exit /b %errorlevel%

%exepath%update tmp.aes %com% -250000 %reboot%
del tmp.aes /q

exit /B %errorlevel%

:not_exists
	echo ERROR: file [%1] or [%1.hex] does not exist
	exit /B 1

:no_argument
	echo ERROR: no argument specified
	exit /B 1


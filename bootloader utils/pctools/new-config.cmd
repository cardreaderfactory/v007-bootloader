@echo off
setlocal

set config="aesconfig.txt"

if not exist %config% (
	echo config file (%config%) does not exist. generating one....
	genconf %config%
) else (
	echo config file (%config%) exists. 
	echo Generating the .h files based on this config.
)	

gcreate.exe -c %config% -h bootldr.h -k aeskeys.h

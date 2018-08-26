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

HOW TO USE THE PC TOOLS

1. Use the GENCONF application to create a template
   configuration file.
 
   pctools\genconf.exe aesconfig.txt

2. Use the CREATE application to generate the files 'aeskeys.h'
   and 'bootldr.h' to be used when building the bootloader.

   pctools\create.exe -c aesconfig.txt -h avr\bootldr.h -k avr\aeskeys.h

3. Use the CREATE application to encrypt your data.

   pctools\create.exe -c aesconfig.txt -f firmware.hex -o firmware.aes -n

4. [Optional] Edit the bootloader sources to modify any hardware pins

5. Compile the AES bootloader

   cd avr\default
   make

6. Program the bootloader

   prog-firmware.cmd aes.hex

7. Use the UPDATE application to communicate with the bootloader.

   update firmware.aes -COM3 -250000


THE DIRECTORIES

avr/       Source code for avr bootloader
create/		 Source code for 'create'-application
genconf/	 Source code for 'genconf'-application
reference/ The original files from Atmel and AVRFreaks.
update/		 Source code for 'update'-application
pctools/	 Binaries for the applications

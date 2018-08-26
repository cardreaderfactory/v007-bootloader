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

HOW TO BUILD THE BOOTLOADER

1. Use the genconf to create the files 'aeskeys.h' and 'bootldr.h'.

   pctools\genconf.exe conf.txt

2. Open the project in Avr Studio. 
   Under project configuration set the following 4 parameters:
  * Device
  * Frequency
  * Baud rate
  * Boot start address 
  
3. Choose 'Rebuild All'.

4. Program the device with the resulting HEX file.


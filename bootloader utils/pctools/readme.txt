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

Other usefull commands:
-----------------------

1. To create the .h files require to compile the aes bootloader and to generate the firmware for distribution you will need to use gcreate

gcreate -c aesconfig.txt -h bootldr.h -k aeskeys.h
gcreate -c aesconfig.txt -f smth.hex -n -o smth.aes


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

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "aesglobal.h"
#include "bootldr.h"
#include "loader.h"
#include "bus.h"
#include "crc.h"
#include "../../v007/global.h"

//=============================================================================
// Starts here!
//=============================================================================

/**
 *
 * The bootloader is disabled by default, so we can program the
 * firmware together with the bootloader and the device will
 * work.
 *
 * ACTIVATION:
 *  1. By writing the eeprom when programming the bootloader.
 *  2. When the app section fails the CRC check. This will also
 * hang the bootloader so it won't result in an endless loop
 * that rewrites the eeprom continuously (when the cable is not
 *  plugged in). This hang is in here because there is no more
 *  space to write any other instruction.
 *  3. By the application.
 *
 *  DEACTIVATION:
 *
 *  The bootloader can be deactivated after a successful
 *  programming.
 *
 *
 */

int main(void)
{
    /** seting up the reset pin is not really required because if
     *  the device is rebooting during firmware update, it can
     *  always be programed later; however it is a good idea to
     *  set set it in order to avoid troubles */
    PORTC = PORTC_VALUE;

    // enable pullup - give port time to settle
    busInit();

    // Loop forever (loops only if Application Section is damaged)
    for(;;)
    {
        if (eeprom_read_byte((uint8_t*)BOOTLOADER_FLAG_ADDR) == BOOTLOADER_FLAG_ENABLE)
        {
            loader();
        }


    #if defined(CRC_CHECK)
        // Check that the Application Section contents is undamaged
        // by calculating the CRC of the whole memory.
        {
            #ifndef RAMPZ
                uint16_t p = 0x000000;
            #else
                uint32_t p = 0x000000;
            #endif

            uint16_t crc = 0;

            do
            {
            #ifndef RAMPZ
                crc = CRC(crc, pgm_read_byte(p));
            #else
                crc = CRC(crc, pgm_read_byte_far(p));
            #endif
            }
            while (++p < MEM_SIZE);

            // Application Section damaged or forced into bootloader mode by connector
            //   -> do not jump to Reset Vector of the Application Section
            if(crc)
            {
                eeprom_write_byte((uint8_t*)BOOTLOADER_FLAG_ADDR, BOOTLOADER_FLAG_ENABLE);
                for(;;); /* hang to avoid executing invalid apps or rewrite eeprom when the cable is unplugged */
            }
        }
    #endif

        asm volatile ("jmp 0");
    }

}



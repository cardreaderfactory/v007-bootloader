###############################################################################
# Makefile for the project aes
###############################################################################

## General Flags
PROJECT = aes
MCU = atmega168
TARGET = aes.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -fno-tree-scev-cprop -mcall-prologues -fno-inline-small-functions   -DBAUD=250000UL          -DF_CPU=8000000UL -Os -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
# atmega168v, 2kb bootloader = 0x3800 (16384 - 2048 = 14336 (0x3800) )
# atmega328p, 2kb bootloader = 0x7800 (32768 - 2048 = 30720 (0x7800) )
# atmega328p, 4kb bootloader = 0x7000 (32768 - 4096 = 28672 (0x7000) )
LDFLAGS += -nostartfiles  -Wl,--section-start=.text=0x3800 -Wl,-Map=aes.map
# ... and change the value in bootldr.h file (MEM_SIZE = 14336 ) !!!

# also, ensure that you program the bootloader with the correct fuses!

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = aes.o bootldr.o usart.o sys_startup.o aesflash.o crc.o loader.o

## Objects explicitly added by the user
LINKONLYOBJECTS =

## Build
all: $(TARGET) aes.hex aes.eep aes.lss size

## Compile
aes.o: ../aes.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bootldr.o: ../bootldr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usart.o: ../usart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sys_startup.o: ../sys_startup.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

aesflash.o: ../aesflash.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

crc.o: ../crc.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

loader.o: ../loader.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) aes.elf dep/* aes.hex aes.eep aes.lss aes.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


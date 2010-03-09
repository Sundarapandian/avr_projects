# CC = $(CROSS_COMPILE)gcc
# OBJCOPY = $(CROSS_COMPILE)objcopy
# CFLAGS += -mmcu=$(MCU) -I.
# 
# all: lcd_test.hex
# 
# lcd_test.hex: lcd_test.elf
#	$(OBJCOPY) -O $(AVR_BIN_FORMAT) -R .eeprom $< $@
#
# lcd_test.elf: test.o
#	$(CC) $(LDFLAGS) test.o 16x2LCD.o -o $@
#
# test.o: 16x2LCD.o
#
# 16x2LCD.o: 16x2LCD.h

OSDIR = ../../mainboard/os
CROSS_COMPILE ?= avr-
MCU ?= atmega32
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
CFLAGS = -I $(OSDIR)/inc -mmcu=$(MCU) -I $(OSDIR)/ports/avr -I $(OSDIR)/ports/avr/default -I.

TARGET = ioboard
SRC = ioboard.c

# Define all object files.
OBJ = $(SRC:%.c=%.o)

all: lib$(TARGET).a

lib$(TARGET).a: $(OBJ)
	$(AR) r $@ $^

.PHONY: clean

clean:
	rm -f lib$(TARGET).a $(OBJ)

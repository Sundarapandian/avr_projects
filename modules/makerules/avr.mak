include $(DIR_CONFIG)/config.mak

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = wine /mnt/sda5/Applications/WinAVR-20080430/bin/avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf

# Place -D or -U options here for C sources
CFLAGS  += -mmcu=$(MCU) -I$(DIR_CONFIG) -I.
CFLAGS  += $(foreach i,$(AVR_LIBS),-I$(TOP_DIR)/$i/inc)

# Place -D or -U options here for ASM sources
ASFLAGS += -DF_CPU=$(F_CPU) -mmcu=$(MCU)


# Place -D or -U options here for C++ sources
CXXFLAGS += -DF_CPU=$(F_CPU)UL -mmcu=$(MCU)

LIBS += $(foreach i,$(AVR_LIBS),-l$i)

all: $(AVR_LIBS) $(TYPE)
include $(TOP_DIR)/makerules/common.mak

LDFLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL \
	   -Wl,-Map=$(OBJDIR)/$(TARGET).map,--cref \
	   -L$(OUTPUT_DIR)/lib

hex: elf $(OUTPUT_DIR)/bin/$(TARGET).hex
$(OUTPUT_DIR)/bin/$(TARGET).hex: $(ELFTARGET)
	@echo
	@echo "     OBJCOPY   $@"
	@$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock $< $@

COFFCONVERT = $(OBJCOPY) --debugging
COFFCONVERT += --change-section-address .data-0x800000
COFFCONVERT += --change-section-address .bss-0x800000
COFFCONVERT += --change-section-address .noinit-0x800000
COFFCONVERT += --change-section-address .eeprom-0x810000
coff: $(ELFTARGET)
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof


$(AVR_LIBS):
	@$(MAKE) -C $(TOP_DIR)/$@ DIR_CONFIG=$(DIR_CONFIG) TOP_DIR=$(TOP_DIR)


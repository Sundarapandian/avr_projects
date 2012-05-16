########## ALL THE CONFIGURABLE PARAMETERS ###############

#directories
PRJ_DIR := $(TOP_DIR)/acspeed/tst
DIR_CONFIG := $(PRJ_DIR)/cfg
OUTPUT_DIR = $(PRJ_DIR)/build

# MCU name
MCU = atmega32


# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. You can then use this symbol in your source code to 
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
F_CPU = 8000000

# Baudrates for various UARTs
BAUDRATES = -DBAUD=9600UL

# Build type RELEASE/DEBUG
BUILD ?= RELEASE

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = 2


PORT    = avr
EXT_ASM = .S

# UART termination char for getchar!
CFLAGS += -DUART_TCHAR="'\r'"

ifeq '$(BUILD)' 'DEBUG'
CFLAGS += -DF_CPU=$(F_CPU)UL -O0 -gstabs
else
CFLAGS += -DF_CPU=$(F_CPU)UL -O$(OPT)
endif

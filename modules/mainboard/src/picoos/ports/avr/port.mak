#  Copyright (c) 2004, Dennis Kuschel / Swen Moczarski
#  All rights reserved. 
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   3. The name of the author may not be used to endorse or promote
#      products derived from this software without specific prior written
#      permission. 
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
#  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
#  INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
#  OF THE POSSIBILITY OF SUCH DAMAGE.


#  This file is originally from the pico]OS realtime operating system
#  (http://picoos.sourceforge.net).
#
#  $Id: port.mak,v 1.4 2008/08/21 08:05:10 smocz Exp $


# Define the MCU
ifeq '$(strip $(MCU))' ''
$(warning MCU not specified. Now using MCU=atmega32)
MCU = atmega32
endif

# Set to 1 to include generic pico]OS "findbit" function
GENERIC_FINDBIT = 0

# Define extensions
EXT_C   = .c
EXT_ASM = .S
EXT_INT = .i
EXT_OBJ = .o
EXT_LIB = .a
EXT_OUT = .elf


#----------------------------------------------------------------------------
#  GNU C
#

# Define tools: compiler, assembler, archiver, linker
CC = avr-gcc
AS = avr-gcc
AR = avr-ar
LD = avr-gcc

# Define to 1 if CC outputs an assembly file
CC2ASM = 0

# Define to 1 if assembler code must be preprocessed by the compiler
A2C2A  = 0

# Define general options
OPT_CC_INC   = -I
OPT_CC_DEF   = -D
OPT_AS_INC   = -I
OPT_AS_DEF   = -D
OPT_AR_ADD   =
OPT_LD_SEP   =
OPT_LD_PFOBJ =
OPT_LD_PFLIB =
OPT_LD_FIRST =
OPT_LD_LAST  =

# Set global defines for compiler / assembler
CDEFINES = GCC
ADEFINES = GCC

# Set global includes
CINCLUDES = .
AINCLUDES = .

# Distinguish between build modes
ifeq '$(BUILD)' 'DEBUG'
  CFLAGS   += -O0 -g
  AFLAGS   += -g
  CDEFINES += _DBG
  ADEFINES += _DBG
else
  CFLAGS   += -Os
  CDEFINES += _REL
  ADEFINES += _REL
endif

# Define Compiler Flags
CFLAGS += -mmcu=$(MCU)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes -c -o


# Define Assembler Flags
ASFLAGS = -c -mmcu=$(MCU) -Wa,-gstabs 
ASFLAGS += -x assembler-with-cpp -o


# Define Linker Flags
#  -Wl   : pass arguments to the linker
#  -Map  : create a map file
#  --cref: add cross reference to the map file
LDFLAGS  = -mmcu=$(MCU)
LDFLAGS += -Wl,-Map,$(DIR_OUT)/$(TARGET).map,--cref -o 

# Define archiver flags
ARFLAGS = r 

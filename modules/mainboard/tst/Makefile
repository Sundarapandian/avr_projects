TOP_DIR := $(shell readlink -f ../..)
SRC = main.c
TARGET = mbtest
TYPE := hex
AVR_LIBS := mainboard picoos

LDFLAGS = -L$(OUTPUT_DIR)/avr-rel/lib

include cfg/config.mak
include ../../makerules/avr.mak


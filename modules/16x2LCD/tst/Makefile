TOP_DIR_WIN := $(subst \,/,$(shell pwd))/../..
TOP_DIR := $(subst //,/,$(TOP_DIR_WIN))
# TOP_DIR := $(shell readlink -f ../..)
SRC = lcd_test.c
TARGET = lcdtest
TYPE := hex
AVR_LIBS := mainboard picoos 16x2LCD


include cfg/config.mak
include $(TOP_DIR)/makerules/avr.mak


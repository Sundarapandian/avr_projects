TOP_DIR_WIN := $(subst \,/,$(shell pwd))/../..
TOP_DIR := $(subst //,/,$(TOP_DIR_WIN))
#TOP_DIR := $(shell readlink -f ../..)
SRC = irtest.c
TARGET = irtest
TYPE := hex
AVR_LIBS := mainboard picoos ir


include cfg/config.mak
include ../../makerules/avr.mak


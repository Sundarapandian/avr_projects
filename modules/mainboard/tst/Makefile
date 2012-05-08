TOP_DIR_WIN := $(subst \,/,$(shell pwd))/../..
TOP_DIR := $(subst //,/,$(TOP_DIR_WIN))
#TOP_DIR := $(shell readlink -f ../..)
SRC = mbtest.c
TARGET = mbtest
TYPE := hex
AVR_LIBS := mainboard picoos


include cfg/config.mak
include ../../makerules/avr.mak


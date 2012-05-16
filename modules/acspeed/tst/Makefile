TOP_DIR_WIN := $(subst \,/,$(shell pwd))/../..
TOP_DIR := $(subst //,/,$(TOP_DIR_WIN))
# TOP_DIR := $(shell readlink -f ../..)
SRC = acspeed_tst.c
TARGET = acspeedtst
TYPE := hex
AVR_LIBS := mainboard picoos acspeed


include cfg/config.mak
include $(TOP_DIR)/makerules/avr.mak


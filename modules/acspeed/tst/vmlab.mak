SHELL := cmd.exe
CP := copy
all:
	$(MAKE) SHELL=cmd.exe EXTRA_CFLAGS=-DSIMULATION BUILD=DEBUG all
	$(MAKE) SHELL=cmd.exe BUILD=DEBUG coff
	$(CP) build\bin\*.hex .

.PHONY: clean
clean:
	-@del /S /Q build
	-@rmdir /Q build
	-@del /Q *.cof
	-@del /Q *.hex



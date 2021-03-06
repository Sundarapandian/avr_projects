OBJDIR := $(OUTPUT_DIR)/$(TARGET)

VPATH = $(SRC_DIRS)

CFLAGS += -I$(TOP_DIR)/$(TARGET)/inc $(EXTRA_CFLAGS)
# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CXXSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o)

LIBTARGET=$(OUTPUT_DIR)/lib/lib$(TARGET).a
ELFTARGET=$(OUTPUT_DIR)/bin/$(TARGET).elf

lib: $(OBJDIR) $(OUTPUT_DIR)/lib $(OBJ) $(LIBTARGET)
elf: $(OBJDIR) $(OUTPUT_DIR)/bin $(OBJ) $(ELFTARGET)

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo "     LD        $@"
	@$(CC) $(LDFLAGS) $^ --output $@ $(LIBS)

# Create library from object files.
.SECONDARY : $(LIBTARGET)
.PRECIOUS :  $(OBJ)
$(OUTPUT_DIR)/lib/%a: $(OBJ)
	@echo
	@echo "     AR        $@"
	@$(AR) $@ $(OBJ)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo
	@echo "     CC        $<"
	@$(CC) -c $(CFLAGS) $< -o $@ 


# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo "     CXX        $<"
	@$(CC) -c $(CXXFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
$(OBJDIR)/%.s : %.c
	$(CC) -S $(CFLAGS) $< -o $@


# Compile: create assembler files from C++ source files.
$(OBJDIR)/%.s : %.cpp
	$(CC) -S $(CXXFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : %.S
	@echo
	@echo "     AS         $<"
	@$(CC) -c $(ASFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report.
$(OBJDIR)/%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 

$(OBJDIR):
	@echo
	@echo "     MKDIR      $@"
ifeq ($(SHELL), cmd.exe)
	@mkdir $(subst /,\\,$(OBJDIR))
else
	@mkdir -p $(OBJDIR) 2>/dev/null
endif

$(OUTPUT_DIR)/lib:
	@echo
	@echo "     MKDIR      $@"
ifeq ($(SHELL), cmd.exe)
	@mkdir $(subst /,\\,$(OUTPUT_DIR))\lib
else
	@mkdir -p $(OUTPUT_DIR)/lib 2>/dev/null
endif

$(OUTPUT_DIR)/bin:
	@echo
	@echo "     MKDIR      $@"
ifeq ($(SHELL), cmd.exe)
	@mkdir $(subst /,\\,$(OUTPUT_DIR))\bin
else
	@mkdir -p $(OUTPUT_DIR)/bin 2>/dev/null
endif

ifeq ($(SHELL), cmd.exe)
$(shell mkdir $(subst /,\\,$(OBJDIR)))
else
$(shell mkdir $(OBJDIR) 2>/dev/null)
endif

clean:
	@echo Cleaning work area
	-@rm -Rf $(LIBTARGET) $(ELFTARGET) $(OBJS) $(CLEAN_LIST) $(OBJDIR)/*

distclean:
	@echo Removing all build files
	-@rm -Rf build


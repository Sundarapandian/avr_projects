OBJDIR := $(OUTPUT_DIR)/$(TARGET)

VPATH = $(SRC_DIRS)

CFLAGS += -I$(TOP_DIR)/$(TARGET)/inc
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
	@mkdir -p $(OBJDIR)

$(OUTPUT_DIR)/lib:
	@echo
	@echo "     MKDIR      $@"
	@mkdir -p $(OUTPUT_DIR)/lib 2> /dev/null

$(OUTPUT_DIR)/bin:
	@echo
	@echo "     MKDIR      $@"
	@mkdir -p $(OUTPUT_DIR)/bin 2>/dev/null

$(shell mkdir $(OBJDIR) 2>/dev/null)

clean:
	@echo Cleaning work area
	-@rm -f $(LIBTARGET) $(ELFTARGET) $(OBJS) $(CLEAN_LIST) $(OBJDIR)/*


.PHONY : all clean test

# Toolchain target
T_TARGET=i686-elf
# Toolchain dir
T_DIR=./toolchain/
# Toolchain binaries
T_BIN_DIR=$(T_DIR)/bin
# Build dir
B_DIR=./build/
# Source dir
S_DIR=./src/

LINKER_FILE=linker.ld

AS=$(T_BIN_DIR)/$(T_TARGET)-as
CC=$(T_BIN_DIR)/$(T_TARGET)-gcc
LD=$(T_BIN_DIR)/$(T_TARGET)-ld

WARN_FLAGS=-Wall -Wextra
CSTANDARD=-std=gnu99
CFLAGS=$(WARN_FLAGS) $(CSTANDARD) -ffreestanding -nostdlib

KERNEL=9kernel
KERNEL_BIN=$(B_DIR)/$(KERNEL).bin
KERNEL_SOURCE=$(S_DIR)/kernel.c
KERNEL_OBJ=$(B_DIR)/kernel.o

KUTIL_SOURCE=$(S_DIR)/kernel_utility.c
KUTIL_OBJ=$(B_DIR)/kernel_utility.o

BOOT_OBJ=$(B_DIR)/boot.o
BOOT_SOURCE=$(S_DIR)/boot.s

# Set up everything
all: $(B_DIR) $(KERNEL_BIN) 
	@echo "Done!"

# Make sure the build directory exists, else make it
$(B_DIR):
	mkdir $(B_DIR)

# Link into the kernel file
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(KUTIL_OBJ)
	$(CC) -T $(LINKER_FILE) -B $(T_DIR)\
		-o $(KERNEL_BIN) $(BOOT_OBJ) $(KERNEL_OBJ) $(KUTIL_OBJ)\
		-lgcc $(CFLAGS)

# Compile kernel
$(KERNEL_OBJ): $(KERNEL_SOURCE)
	# No -O2 yet
	$(CC) -c $(KERNEL_SOURCE) -o $(KERNEL_OBJ) $(CFLAGS)

# Compile extra utilities
$(KUTIL_OBJ): $(KUTIL_SOURCE)
	$(CC) -c $(KUTIL_SOURCE) -o $(KUTIL_OBJ) $(CFLAGS)

# Assemble kernel bootstrap stub
$(BOOT_OBJ): $(BOOT_SOURCE)
	$(AS) -o $(BOOT_OBJ) $(BOOT_SOURCE)

clean:
	rm $(B_DIR)/* -f

test:
	@grub-file --is-x86-multiboot $(KERNEL_BIN)
	@if [ -z $? ]; then\
		echo "Valid multiboot kernel";\
	else\
		echo "Invalid multiboot kernel";\
	fi

.PHONY : all

# Toolchain target
T_TARGET="i686-elf"
# Toolchain dir
T_DIR=./toolchain/
# Toolchain binaries
T_BIN_DIR=$(T_DIR)/bin
# Build dir
B_DIR=./build/
# Source dir
S_DIR=./src/

AS=$(T_BIN_DIR)/$(T_TARGET)-as
CC=$(T_BIN_DIR)/$(T_TARGET)-gcc
LD=$(T_BIN_DIR)/$(T_TARGET)-ld

# Set up everything
all: $(B_DIR) $(B_DIR)/boot.o $(B_DIR)/kernel.o
	@echo "Done!"

# Make sure the build directory exists, else make it
$(B_DIR):
	mkdir $(B_DIR)

# Assemble kernel bootstrap stub
$(B_DIR)/boot.o: $(S_DIR)/boot.s
	$(AS) -o $@ $<

$(B_DIR)/kernel.o: $(S_DIR)/kernel.c
	# No -O2 yet
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -Wall -Wextra

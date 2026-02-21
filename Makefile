# MorgOS Makefile
# Build system for x86_64 kernel

# Toolchain configuration
AS := as
CC := gcc
LD := ld

# Compiler flags
ASFLAGS := --64
CFLAGS := -m64 -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone \
          -mno-mmx -mno-sse -mno-sse2 -mcmodel=kernel -Wall -Wextra -O2
LDFLAGS := -nostdlib -n -T src/boot/linker.ld

# Directories
BUILD_DIR := build
SRC_DIR := src
BOOT_DIR := $(SRC_DIR)/boot
KERNEL_DIR := $(SRC_DIR)/kernel

# Output files
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
ISO_FILE := $(BUILD_DIR)/morgos.iso

# Source files
BOOT_ASM := $(BOOT_DIR)/boot.S
KERNEL_C := $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_OBJS := $(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_C))
BOOT_OBJ := $(BUILD_DIR)/boot.o

# All object files
ALL_OBJS := $(BOOT_OBJ) $(KERNEL_OBJS)

# Default target
.PHONY: all
all: $(KERNEL_ELF)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Build boot object
$(BOOT_OBJ): $(BOOT_ASM) | $(BUILD_DIR)
	@echo "[AS] $<"
	@$(AS) $(ASFLAGS) $< -o $@

# Build kernel object files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c | $(BUILD_DIR)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL_ELF): $(ALL_OBJS)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) -o $@ $(ALL_OBJS)
	@echo "Kernel built successfully: $@"

# Build ISO image
.PHONY: iso
iso: $(KERNEL_ELF)
	@echo "Building ISO image..."
	@bash tools/build-iso.sh

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# Run with QEMU (requires QEMU installed)
.PHONY: run
run: $(KERNEL_ELF)
	@echo "Starting QEMU..."
	@qemu-system-x86_64 -kernel $(KERNEL_ELF) -m 1024 -serial stdio

# Run ISO with QEMU
.PHONY: run-iso
run-iso: iso
	@echo "Starting QEMU with ISO..."
	@qemu-system-x86_64 -cdrom $(ISO_FILE) -m 1024 -serial stdio

# Display help
.PHONY: help
help:
	@echo "MorgOS Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build kernel (default)"
	@echo "  iso       - Build bootable ISO image"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Run kernel with QEMU (direct kernel boot)"
	@echo "  run-iso   - Run ISO with QEMU"
	@echo "  help      - Display this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - GNU Make"
	@echo "  - GCC (cross-compiler recommended: x86_64-elf-gcc)"
	@echo "  - GNU Assembler (as)"
	@echo "  - GNU Linker (ld)"
	@echo "  - xorriso and grub (for ISO building)"
	@echo "  - QEMU (for testing)"

# Prevent make from deleting intermediate files
.PRECIOUS: $(BUILD_DIR)/%.o

#!/bin/bash
# MorgOS ISO Builder Script
# Creates a bootable UEFI/BIOS ISO image

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
ISO_DIR="$BUILD_DIR/iso"
GRUB_DIR="$ISO_DIR/boot/grub"

echo "=== MorgOS ISO Builder ==="
echo "Project root: $PROJECT_ROOT"
echo "Build directory: $BUILD_DIR"

# Check if kernel is built
if [ ! -f "$BUILD_DIR/kernel.elf" ]; then
    echo "Error: Kernel not found. Please run 'make all' first."
    exit 1
fi

# Create ISO directory structure
echo "Creating ISO directory structure..."
mkdir -p "$GRUB_DIR"
mkdir -p "$ISO_DIR/boot"

# Copy kernel
echo "Copying kernel..."
cp "$BUILD_DIR/kernel.elf" "$ISO_DIR/boot/kernel.elf"

# Create GRUB configuration
echo "Creating GRUB configuration..."
cat > "$GRUB_DIR/grub.cfg" << 'EOF'
set timeout=5
set default=0

menuentry "MorgOS v0.1" {
    multiboot2 /boot/kernel.elf
    boot
}

menuentry "MorgOS v0.1 (Safe Mode)" {
    multiboot2 /boot/kernel.elf
    boot
}
EOF

# Check for grub-mkrescue
if ! command -v grub-mkrescue &> /dev/null; then
    echo "Warning: grub-mkrescue not found. Trying xorriso directly..."
    
    # Alternative: create ISO with xorriso
    if command -v xorriso &> /dev/null; then
        echo "Creating ISO with xorriso..."
        xorriso -as mkisofs \
            -R -J -c boot.cat \
            -b boot/grub/i386-pc/eltorito.img \
            -no-emul-boot -boot-load-size 4 -boot-info-table \
            -eltorito-alt-boot \
            -e boot/grub/efi.img \
            -no-emul-boot \
            -o "$BUILD_DIR/morgos.iso" \
            "$ISO_DIR" 2>/dev/null || true
    else
        echo "Error: Neither grub-mkrescue nor xorriso found."
        echo "Please install GRUB and xorriso to build ISO images."
        exit 1
    fi
else
    # Create bootable ISO with grub-mkrescue
    echo "Creating bootable ISO with grub-mkrescue..."
    grub-mkrescue -o "$BUILD_DIR/morgos.iso" "$ISO_DIR" 2>/dev/null || {
        echo "Warning: grub-mkrescue failed. ISO may be incomplete."
        exit 1
    }
fi

# Check if ISO was created
if [ -f "$BUILD_DIR/morgos.iso" ]; then
    ISO_SIZE=$(du -h "$BUILD_DIR/morgos.iso" | cut -f1)
    echo ""
    echo "=== ISO Build Complete ==="
    echo "ISO file: $BUILD_DIR/morgos.iso"
    echo "Size: $ISO_SIZE"
    echo ""
    echo "To test with QEMU:"
    echo "  qemu-system-x86_64 -cdrom $BUILD_DIR/morgos.iso -m 1024"
    echo ""
else
    echo "Error: ISO file was not created."
    exit 1
fi

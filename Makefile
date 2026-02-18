CC = gcc
ASM = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -Wall -Wextra -c
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

KERNEL = mertOS.bin
ISO = mertOS.iso

C_SOURCES = $(wildcard src/kernel/*.c) $(wildcard src/drivers/*.c) $(wildcard src/lib/*.c) $(wildcard src/shell/*.c)
ASM_SOURCES = $(wildcard src/boot/*.asm)

C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.asm=.o)

OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

all: $(ISO)

$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL)
	cp $(KERNEL) iso/boot/$(KERNEL)
	grub-mkrescue -o $(ISO) iso/

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
	rm -f iso/boot/$(KERNEL)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

run-debug: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -d int -no-reboot

.PHONY: all clean run run-debug

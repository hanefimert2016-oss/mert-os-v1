CC = gcc
ASM = nasm
LD = ld

CFLAGS = -m64 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs \
         -mno-red-zone -mcmodel=large -Wall -Wextra -c
ASMFLAGS = -f elf64
LDFLAGS = -m elf_x86_64 -T linker.ld -nostdlib

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
	qemu-system-x86_64 -cdrom $(ISO) -m 256M

run-vnc: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 256M -display none -vnc 0.0.0.0:0

.PHONY: all clean run run-vnc

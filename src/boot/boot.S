/*
 * MorgOS Bootloader - x86_64 Assembly
 * 
 * Boot sequence: 16-bit real mode -> 32-bit protected mode -> 64-bit long mode
 * UEFI priority, with legacy BIOS support
 */

.code16
.section .boot, "awx"

# Multiboot2 header for GRUB compatibility
.align 8
multiboot2_header_start:
    .long 0xe85250d6                    # Multiboot2 magic
    .long 0                             # Architecture: i386
    .long multiboot2_header_end - multiboot2_header_start
    .long -(0xe85250d6 + 0 + (multiboot2_header_end - multiboot2_header_start))
    
    # End tag
    .align 8
    .short 0    # type
    .short 0    # flags
    .long 8     # size
multiboot2_header_end:

.global _start
_start:
    cli                                 # Disable interrupts
    cld                                 # Clear direction flag

    # Set up stack (below 1MB)
    movw $0x7c00, %sp
    xorw %ax, %ax
    movw %ax, %ss

    # Enable A20 line (for accessing memory above 1MB)
    call enable_a20

    # Load GDT for protected mode
    lgdt gdt_descriptor

    # Enter protected mode
    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0

    # Far jump to 32-bit code segment
    ljmp $0x08, $protected_mode

enable_a20:
    # Fast A20 enable via keyboard controller
    inb $0x92, %al
    orb $2, %al
    outb %al, $0x92
    ret

.code32
.align 4
protected_mode:
    # Set up segment registers for 32-bit mode
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Set up stack in 32-bit mode
    movl $0x90000, %esp

    # Check for long mode support (CPUID)
    call check_long_mode_support

    # Set up paging for long mode
    call setup_paging

    # Load 64-bit GDT
    lgdt gdt64_descriptor

    # Enable PAE (Physical Address Extension)
    movl %cr4, %eax
    orl $(1 << 5), %eax
    movl %eax, %cr4

    # Set CR3 to PML4 table address
    movl $pml4_table, %eax
    movl %eax, %cr3

    # Enable long mode (set EFER.LME)
    movl $0xC0000080, %ecx
    rdmsr
    orl $(1 << 8), %eax
    wrmsr

    # Enable paging and protected mode
    movl %cr0, %eax
    orl $(1 << 31) | 1, %eax
    movl %eax, %cr0

    # Far jump to 64-bit code segment
    ljmp $0x08, $long_mode

check_long_mode_support:
    # Check if CPUID is supported
    pushfl
    pop %eax
    movl %eax, %ecx
    xorl $(1 << 21), %eax
    pushl %eax
    popfl
    pushfl
    pop %eax
    pushl %ecx
    popfl
    cmpl %ecx, %eax
    je .no_long_mode

    # Check for extended CPUID
    movl $0x80000000, %eax
    cpuid
    cmpl $0x80000001, %eax
    jb .no_long_mode

    # Check for long mode
    movl $0x80000001, %eax
    cpuid
    testl $(1 << 29), %edx
    jz .no_long_mode
    ret

.no_long_mode:
    # Print error message (simplified)
    movl $0xb8000, %edi
    movl $0x4f4e4f4f, (%edi)  # "ON" in red
    hlt

setup_paging:
    # Clear page tables
    movl $pml4_table, %edi
    movl %edi, %cr3
    xorl %eax, %eax
    movl $4096, %ecx
    rep stosl
    movl %cr3, %edi

    # Set up PML4[0] -> PDPT
    movl $pdp_table, %eax
    orl $0x3, %eax              # Present + Writable
    movl %eax, (%edi)

    # Set up PDPT[0] -> PD
    movl $pd_table, %eax
    orl $0x3, %eax
    movl $pdp_table, %edi
    movl %eax, (%edi)

    # Set up PD[0-3] -> Identity map first 8MB with 2MB pages
    movl $pd_table, %edi
    movl $0x83, %eax            # Present + Writable + Huge (2MB)
    movl $4, %ecx
.setup_pd_loop:
    movl %eax, (%edi)
    addl $0x200000, %eax        # Next 2MB
    addl $8, %edi
    loop .setup_pd_loop

    ret

.code64
.align 8
long_mode:
    # Set up segment registers for 64-bit mode
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Set up 64-bit stack
    movq $0x90000, %rsp

    # Jump to kernel main
    movabs $kernel_main, %rax
    call *%rax

    # Halt if kernel returns
.halt_loop:
    hlt
    jmp .halt_loop

# GDT for 32-bit protected mode
.align 16
gdt_start:
    .quad 0x0000000000000000    # Null descriptor
    .quad 0x00cf9a000000ffff    # Code segment (32-bit)
    .quad 0x00cf92000000ffff    # Data segment (32-bit)
gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1
    .long gdt_start

# GDT for 64-bit long mode
.align 16
gdt64_start:
    .quad 0x0000000000000000    # Null descriptor
    .quad 0x00af9a000000ffff    # Code segment (64-bit)
    .quad 0x00af92000000ffff    # Data segment (64-bit)
gdt64_end:

gdt64_descriptor:
    .word gdt64_end - gdt64_start - 1
    .long gdt64_start

# Page tables (4KB aligned)
.align 4096
pml4_table:
    .skip 4096
pdp_table:
    .skip 4096
pd_table:
    .skip 4096

.section .note.GNU-stack,"",@progbits

; MertOS Boot Assembly - x86_64 Multiboot2 Compatible Boot Code
; Sets up multiboot header, GDT, stack, and jumps to kernel_main

section .multiboot_header
header_start:
    dd 0xe85250d6                ; multiboot2 magic number
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; checksum

    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .bss
align 16
stack_bottom:
    resb 16384  ; 16 KB stack
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    push 0
    popf

    push ebx    ; multiboot info pointer
    push eax    ; multiboot magic number

    call kernel_main

    cli
.hang:
    hlt
    jmp .hang

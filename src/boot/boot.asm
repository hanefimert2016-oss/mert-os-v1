section .multiboot_header
align 8
header_start:
    dd 0xe85250d6
    dd 0
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    align 8
    dw 5
    dw 0
    dd 20
    dd 1024
    dd 768
    dd 32

    align 8
    dw 0
    dw 0
    dd 8
header_end:

section .bss
align 4096
pml4:
    resb 4096
pdpt:
    resb 4096
pd0:
    resb 4096
pd1:
    resb 4096
pd2:
    resb 4096
pd3:
    resb 4096

align 16
stack_bottom:
    resb 32768
stack_top:

section .data
align 4
mb_magic: dd 0
mb_info:  dd 0

align 16
gdt64:
    dq 0
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.data: equ $ - gdt64
    dq (1<<41) | (1<<44) | (1<<47)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
global _start
extern kernel_main

[BITS 32]
_start:
    mov [mb_magic], eax
    mov [mb_info], ebx
    cli

    mov edi, pml4
    xor eax, eax
    mov ecx, 6 * 4096 / 4
    rep stosd

    mov eax, pdpt
    or eax, 0x03
    mov [pml4], eax

    mov eax, pd0
    or eax, 0x03
    mov [pdpt], eax
    mov eax, pd1
    or eax, 0x03
    mov [pdpt + 8], eax
    mov eax, pd2
    or eax, 0x03
    mov [pdpt + 16], eax
    mov eax, pd3
    or eax, 0x03
    mov [pdpt + 24], eax

    mov edi, pd0
    mov eax, 0x83
    mov ecx, 2048
.fill_pd:
    mov [edi], eax
    mov dword [edi + 4], 0
    add eax, 0x200000
    jnc .no_carry
    mov dword [edi + 12], 0
    add edi, 8
    dec ecx
    jmp .fill_check
.no_carry:
    add edi, 8
    dec ecx
.fill_check:
    test ecx, ecx
    jnz .fill_pd

    mov eax, pml4
    mov cr3, eax

    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    lgdt [gdt64.pointer]

    jmp gdt64.code:long_mode_start

[BITS 64]
long_mode_start:
    mov ax, gdt64.data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, stack_top

    xor rdi, rdi
    mov edi, [mb_magic]
    xor rsi, rsi
    mov esi, [mb_info]

    call kernel_main

    cli
.hang:
    hlt
    jmp .hang

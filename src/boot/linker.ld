/*
 * MorgOS Kernel Linker Script
 * x86_64 architecture
 */

ENTRY(_start)

SECTIONS
{
    /* Kernel starts at 1MB */
    . = 1M;

    /* Boot section */
    .boot ALIGN(4K) : AT(ADDR(.boot))
    {
        *(.boot)
    }

    /* Text section (code) */
    .text ALIGN(4K) : AT(ADDR(.text))
    {
        *(.text)
        *(.text.*)
    }

    /* Read-only data */
    .rodata ALIGN(4K) : AT(ADDR(.rodata))
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data */
    .data ALIGN(4K) : AT(ADDR(.data))
    {
        *(.data)
        *(.data.*)
    }

    /* BSS (uninitialized data) */
    .bss ALIGN(4K) : AT(ADDR(.bss))
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    /* Stack */
    .stack ALIGN(4K) : AT(ADDR(.stack))
    {
        . = . + 16K;
        kernel_stack_top = .;
    }

    /* Discard sections */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.GNU-stack)
    }
}

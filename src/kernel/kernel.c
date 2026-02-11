#include "gdt.h"
#include "idt.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"
#include "../lib/string.h"

static void print_welcome(void) {
    vga_print_color("  __  __           _    ___  ____  \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" |  \\/  | ___ _ __| |_ / _ \\/ ___| \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" | |\\/| |/ _ \\ '__| __| | | \\___ \\ \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" | |  | |  __/ |  | |_| |_| |___) |\n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" |_|  |_|\\___|_|   \\__|\\___/|____/ \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print("\n");
    vga_print_color("  MertOS v1.0.0 - x86 Isletim Sistemi\n", VGA_YELLOW, VGA_BLACK);
    vga_print_color("  Gelistirici: Mert\n", VGA_LIGHT_GREEN, VGA_BLACK);
    vga_print("\n");
    vga_print_color("  Hosgeldiniz! 'help' yazarak komutlari gorebilirsiniz.\n", VGA_WHITE, VGA_BLACK);
    vga_print_color("  ================================================\n\n", VGA_DARK_GREY, VGA_BLACK);
}

void kernel_main(uint32_t magic, uint32_t addr) {
    (void)magic;
    (void)addr;

    gdt_init();
    idt_init();
    vga_init();

    __asm__ volatile ("sti");

    keyboard_init();

    print_welcome();
    shell_run();
}

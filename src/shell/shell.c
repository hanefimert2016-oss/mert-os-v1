#include "shell.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../lib/string.h"
#include "../lib/io.h"

static char input_buffer[SHELL_BUFFER_SIZE];
static int input_pos = 0;

static void print_prompt(void) {
    vga_print_color("mertOS", VGA_LIGHT_GREEN, VGA_BLACK);
    vga_print_color("> ", VGA_LIGHT_CYAN, VGA_BLACK);
}

static void cmd_help(void) {
    vga_print_color("\n=== MertOS Komutlar ===\n", VGA_YELLOW, VGA_BLACK);
    vga_print("  help     - Bu yardim mesajini goster\n");
    vga_print("  clear    - Ekrani temizle\n");
    vga_print("  info     - Sistem bilgisi\n");
    vga_print("  reboot   - Sistemi yeniden baslat\n");
    vga_print("  time     - CPU tick sayacini goster\n");
    vga_print("  echo     - Metni ekrana yaz\n");
    vga_print("  color    - Renk testi goster\n");
    vga_print("  logo     - MertOS logosunu goster\n");
    vga_print("  mem      - Bellek bilgisi\n");
    vga_print("  shutdown - Sistemi kapat\n");
}

static void cmd_info(void) {
    vga_print_color("\n=== MertOS Sistem Bilgisi ===\n", VGA_YELLOW, VGA_BLACK);
    vga_print("  OS Adi      : MertOS\n");
    vga_print("  Versiyon    : 1.0.0\n");
    vga_print("  Mimari      : x86 (i386)\n");
    vga_print("  Gelistirici : Mert\n");
    vga_print("  Kernel      : MertOS Kernel\n");
    vga_print("  VGA Mode    : 80x25 Text Mode\n");
}

static void cmd_reboot(void) {
    vga_print_color("\nSistem yeniden baslatiliyor...\n", VGA_LIGHT_RED, VGA_BLACK);
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    __asm__ volatile ("hlt");
}

static void cmd_shutdown(void) {
    vga_print_color("\nSistem kapatiliyor...\n", VGA_LIGHT_RED, VGA_BLACK);
    outb(0x604, 0x2000);
    outb(0x4004, 0x3400);
    __asm__ volatile ("hlt");
}

static void cmd_color(void) {
    vga_print("\n");
    const char *color_names[] = {
        "BLACK", "BLUE", "GREEN", "CYAN",
        "RED", "MAGENTA", "BROWN", "LIGHT_GREY",
        "DARK_GREY", "LIGHT_BLUE", "LIGHT_GREEN", "LIGHT_CYAN",
        "LIGHT_RED", "LIGHT_MAGENTA", "YELLOW", "WHITE"
    };
    for (int i = 0; i < 16; i++) {
        vga_print_color("  ", (uint8_t)i, VGA_BLACK);
        vga_print_color(color_names[i], (uint8_t)i, VGA_BLACK);
        vga_print("\n");
    }
}

static void cmd_echo(const char *args) {
    vga_print("\n");
    if (args && *args) {
        vga_print(args);
    }
    vga_print("\n");
}

static void cmd_logo(void) {
    vga_print("\n");
    vga_print_color("  __  __           _    ___  ____  \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" |  \\/  | ___ _ __| |_ / _ \\/ ___| \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" | |\\/| |/ _ \\ '__| __| | | \\___ \\ \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" | |  | |  __/ |  | |_| |_| |___) |\n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color(" |_|  |_|\\___|_|   \\__|\\___/|____/ \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color("                                    \n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_print_color("  x86 Isletim Sistemi v1.0.0\n", VGA_YELLOW, VGA_BLACK);
    vga_print_color("  Gelistirici: Mert\n\n", VGA_LIGHT_GREEN, VGA_BLACK);
}

static void cmd_time(void) {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    vga_print("\nCPU Tick: ");
    vga_print_hex(hi);
    vga_print_hex(lo);
    vga_print("\n");
}

static void cmd_mem(void) {
    vga_print_color("\n=== Bellek Bilgisi ===\n", VGA_YELLOW, VGA_BLACK);
    vga_print("  Kernel Baslangic : ");
    vga_print_hex(0x100000);
    vga_print("\n");
    vga_print("  VGA Bellek       : ");
    vga_print_hex(0xB8000);
    vga_print("\n");
    vga_print("  Stack Boyutu     : 16 KB\n");
}

static void process_command(void) {
    input_buffer[input_pos] = '\0';

    char *cmd = input_buffer;
    while (*cmd == ' ') cmd++;

    if (*cmd == '\0') {
        return;
    }

    char *args = cmd;
    while (*args && *args != ' ') args++;
    if (*args == ' ') {
        *args = '\0';
        args++;
        while (*args == ' ') args++;
    }

    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd, "clear") == 0) {
        vga_clear();
        return;
    } else if (strcmp(cmd, "info") == 0) {
        cmd_info();
    } else if (strcmp(cmd, "reboot") == 0) {
        cmd_reboot();
    } else if (strcmp(cmd, "shutdown") == 0) {
        cmd_shutdown();
    } else if (strcmp(cmd, "color") == 0) {
        cmd_color();
    } else if (strcmp(cmd, "echo") == 0) {
        cmd_echo(args);
    } else if (strcmp(cmd, "logo") == 0) {
        cmd_logo();
    } else if (strcmp(cmd, "time") == 0) {
        cmd_time();
    } else if (strcmp(cmd, "mem") == 0) {
        cmd_mem();
    } else {
        vga_print_color("\nBilinmeyen komut: ", VGA_LIGHT_RED, VGA_BLACK);
        vga_print(cmd);
        vga_print("\n'help' yazarak komutlari gorebilirsiniz.\n");
    }
}

void shell_init(void) {
    input_pos = 0;
    memset(input_buffer, 0, SHELL_BUFFER_SIZE);
}

void shell_run(void) {
    shell_init();
    print_prompt();

    while (1) {
        char c = keyboard_getchar();

        if (c == '\n') {
            vga_putchar('\n');
            process_command();
            input_pos = 0;
            memset(input_buffer, 0, SHELL_BUFFER_SIZE);
            print_prompt();
        } else if (c == '\b') {
            if (input_pos > 0) {
                input_pos--;
                input_buffer[input_pos] = '\0';
                vga_backspace();
            }
        } else if (input_pos < SHELL_BUFFER_SIZE - 1) {
            input_buffer[input_pos++] = c;
            vga_putchar(c);
        }
    }
}

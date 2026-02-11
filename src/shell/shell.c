#include "shell.h"
#include "../drivers/framebuffer.h"
#include "../drivers/font.h"
#include "../drivers/keyboard.h"
#include "../lib/string.h"
#include "../lib/io.h"

#define BG_COLOR    0x001A1A2E
#define FG_COLOR    0x00E0E0E0
#define PROMPT_CLR  0x0000FF88
#define TITLE_CLR   0x0000DDFF
#define ERR_CLR     0x00FF4444
#define ACCENT_CLR  0x00FFD700
#define CMD_CLR     0x0088CCFF
#define MARGIN_X    10
#define MARGIN_Y    10

static char input_buffer[SHELL_BUFFER_SIZE];
static int input_pos = 0;
static uint32_t cursor_x = MARGIN_X;
static uint32_t cursor_y = MARGIN_Y;
static uint32_t screen_w = 0;
static uint32_t screen_h = 0;
static uint32_t max_cols = 0;
static uint32_t max_rows = 0;

static void shell_putchar(char c, uint32_t color) {
    if (c == '\n') {
        cursor_x = MARGIN_X;
        cursor_y += FONT_HEIGHT;
    } else if (c == '\t') {
        cursor_x += FONT_WIDTH * 4;
    } else {
        font_draw_char(cursor_x, cursor_y, c, color, BG_COLOR);
        cursor_x += FONT_WIDTH;
    }

    if (cursor_x + FONT_WIDTH > screen_w - MARGIN_X) {
        cursor_x = MARGIN_X;
        cursor_y += FONT_HEIGHT;
    }

    if (cursor_y + FONT_HEIGHT > screen_h - MARGIN_Y) {
        struct framebuffer_info *fbi = fb_get_info();
        uint32_t line_bytes = fbi->pitch;
        uint32_t scroll_lines = FONT_HEIGHT;
        uint8_t *fb_ptr = (uint8_t *)fbi->addr;

        uint32_t copy_start = (MARGIN_Y + scroll_lines) * line_bytes;
        uint32_t copy_end = fbi->height * line_bytes;
        uint32_t copy_size = copy_end - copy_start;
        memcpy(fb_ptr + MARGIN_Y * line_bytes, fb_ptr + copy_start, copy_size);

        uint32_t clear_start_y = fbi->height - scroll_lines;
        fb_fill_rect(0, clear_start_y, fbi->width, scroll_lines, BG_COLOR);

        cursor_y -= scroll_lines;
    }
}

static void shell_print(const char *str, uint32_t color) {
    while (*str) {
        shell_putchar(*str++, color);
    }
}

static void shell_print_dec(uint32_t num, uint32_t color) {
    if (num == 0) { shell_putchar('0', color); return; }
    char buf[12];
    int i = 0;
    while (num > 0) { buf[i++] = '0' + (num % 10); num /= 10; }
    for (int j = i - 1; j >= 0; j--) shell_putchar(buf[j], color);
}

static void shell_print_hex(uint32_t num, uint32_t color) {
    shell_print("0x", color);
    char hex[] = "0123456789ABCDEF";
    bool started = false;
    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (num >> i) & 0xF;
        if (nibble || started || i == 0) {
            shell_putchar(hex[nibble], color);
            started = true;
        }
    }
}

static void print_prompt(void) {
    shell_print("mertOS", PROMPT_CLR);
    shell_print("> ", ACCENT_CLR);
}

static void draw_cursor(uint32_t color) {
    fb_fill_rect(cursor_x, cursor_y, FONT_WIDTH, FONT_HEIGHT, color);
}

static void cmd_help(void) {
    shell_print("\n", FG_COLOR);
    shell_print("=== MertOS Komutlar ===\n", ACCENT_CLR);
    shell_print("  help     ", CMD_CLR); shell_print("- Bu yardim mesajini goster\n", FG_COLOR);
    shell_print("  clear    ", CMD_CLR); shell_print("- Ekrani temizle\n", FG_COLOR);
    shell_print("  info     ", CMD_CLR); shell_print("- Sistem bilgisi\n", FG_COLOR);
    shell_print("  reboot   ", CMD_CLR); shell_print("- Sistemi yeniden baslat\n", FG_COLOR);
    shell_print("  shutdown ", CMD_CLR); shell_print("- Sistemi kapat\n", FG_COLOR);
    shell_print("  echo     ", CMD_CLR); shell_print("- Metni ekrana yaz\n", FG_COLOR);
    shell_print("  color    ", CMD_CLR); shell_print("- Renk testi goster\n", FG_COLOR);
    shell_print("  logo     ", CMD_CLR); shell_print("- MertOS logosunu goster\n", FG_COLOR);
    shell_print("  mem      ", CMD_CLR); shell_print("- Bellek bilgisi\n", FG_COLOR);
    shell_print("  gfx      ", CMD_CLR); shell_print("- Grafik testi ciz\n", FG_COLOR);
    shell_print("  time     ", CMD_CLR); shell_print("- CPU tick sayacini goster\n", FG_COLOR);
}

static void cmd_info(void) {
    struct framebuffer_info *fbi = fb_get_info();
    shell_print("\n", FG_COLOR);
    shell_print("=== MertOS Sistem Bilgisi ===\n", ACCENT_CLR);
    shell_print("  OS Adi       : ", FG_COLOR); shell_print("MertOS\n", TITLE_CLR);
    shell_print("  Versiyon     : ", FG_COLOR); shell_print("2.0.0\n", TITLE_CLR);
    shell_print("  Mimari       : ", FG_COLOR); shell_print("x86_64 (Long Mode)\n", TITLE_CLR);
    shell_print("  Gelistirici  : ", FG_COLOR); shell_print("Mert\n", TITLE_CLR);
    shell_print("  Cozunurluk   : ", FG_COLOR);
    shell_print_dec(fbi->width, TITLE_CLR);
    shell_print("x", TITLE_CLR);
    shell_print_dec(fbi->height, TITLE_CLR);
    shell_print("x", TITLE_CLR);
    shell_print_dec(fbi->bpp, TITLE_CLR);
    shell_print("\n", FG_COLOR);
    shell_print("  Framebuffer  : ", FG_COLOR);
    shell_print_hex((uint32_t)(fbi->addr & 0xFFFFFFFF), TITLE_CLR);
    shell_print("\n", FG_COLOR);
    shell_print("  Pitch        : ", FG_COLOR);
    shell_print_dec(fbi->pitch, TITLE_CLR);
    shell_print(" bytes/line\n", FG_COLOR);
    shell_print("  Paging       : ", FG_COLOR); shell_print("4-Level (PML4)\n", TITLE_CLR);
    shell_print("  GDT          : ", FG_COLOR); shell_print("64-bit Long Mode\n", TITLE_CLR);
    shell_print("  IDT          : ", FG_COLOR); shell_print("64-bit (16-byte entries)\n", TITLE_CLR);
}

static void cmd_reboot(void) {
    shell_print("\nSistem yeniden baslatiliyor...\n", ERR_CLR);
    uint8_t good = 0x02;
    while (good & 0x02) good = inb(0x64);
    outb(0x64, 0xFE);
    __asm__ volatile ("hlt");
}

static void cmd_shutdown(void) {
    shell_print("\nSistem kapatiliyor...\n", ERR_CLR);
    outw(0x604, 0x2000);
    __asm__ volatile ("hlt");
}

static void cmd_echo(const char *args) {
    shell_print("\n", FG_COLOR);
    if (args && *args) shell_print(args, FG_COLOR);
    shell_print("\n", FG_COLOR);
}

static void cmd_color(void) {
    shell_print("\n", FG_COLOR);
    uint32_t colors[] = {
        0x00FF0000, 0x0000FF00, 0x000000FF, 0x00FFFF00,
        0x00FF00FF, 0x0000FFFF, 0x00FF8800, 0x008800FF,
        0x00FF4444, 0x0044FF44, 0x004444FF, 0x00FFAA00,
    };
    const char *names[] = {
        "Kirmizi ", "Yesil   ", "Mavi    ", "Sari    ",
        "Magenta ", "Cyan    ", "Turuncu ", "Mor     ",
        "Acik Kr ", "Acik Ys ", "Acik Mv ", "Altin   ",
    };
    for (int i = 0; i < 12; i++) {
        shell_print("  ", FG_COLOR);
        fb_fill_rect(cursor_x, cursor_y + 2, 12, 12, colors[i]);
        cursor_x += 16;
        shell_print(names[i], colors[i]);
        shell_print("\n", FG_COLOR);
    }
}

static void cmd_logo(void) {
    shell_print("\n", FG_COLOR);
    shell_print("  __  __           _    ___  ____  \n", TITLE_CLR);
    shell_print(" |  \\/  | ___ _ __| |_ / _ \\/ ___| \n", TITLE_CLR);
    shell_print(" | |\\/| |/ _ \\'__| __| | | \\___ \\ \n", TITLE_CLR);
    shell_print(" | |  | |  __/ |  | |_| |_| |___) |\n", TITLE_CLR);
    shell_print(" |_|  |_|\\___|_|   \\__|\\___/|____/ \n", TITLE_CLR);
    shell_print("\n", FG_COLOR);
    shell_print("  x86_64 Long Mode Isletim Sistemi v2.0.0\n", ACCENT_CLR);
    shell_print("  Gelistirici: Mert\n", PROMPT_CLR);
}

static void cmd_mem(void) {
    shell_print("\n", FG_COLOR);
    shell_print("=== Bellek Bilgisi ===\n", ACCENT_CLR);
    shell_print("  Kernel Baslangic : ", FG_COLOR); shell_print_hex(0x100000, TITLE_CLR); shell_print("\n", FG_COLOR);
    shell_print("  Stack Boyutu     : ", FG_COLOR); shell_print("32 KB\n", TITLE_CLR);
    shell_print("  Paging Mode      : ", FG_COLOR); shell_print("2MB Huge Pages\n", TITLE_CLR);
    shell_print("  Mapped Memory    : ", FG_COLOR); shell_print("4 GB (identity mapped)\n", TITLE_CLR);
    shell_print("  Page Levels      : ", FG_COLOR); shell_print("PML4 -> PDPT -> PD (2MB)\n", TITLE_CLR);
}

static void cmd_gfx(void) {
    shell_print("\n", FG_COLOR);
    shell_print("Grafik testi ciziliyor...\n", ACCENT_CLR);

    uint32_t bx = MARGIN_X;
    uint32_t by = cursor_y + FONT_HEIGHT + 4;
    uint32_t bw = 300;
    uint32_t bh = 80;

    uint32_t colors[] = {0x00FF0000, 0x0000FF00, 0x000000FF, 0x00FFFF00, 0x00FF00FF};
    for (int i = 0; i < 5; i++) {
        fb_fill_rect(bx + i * 62, by, 58, bh, colors[i]);
    }

    fb_draw_rect(bx - 2, by - 2, bw + 4, bh + 4, 0x00FFFFFF);

    font_draw_string(bx, by + bh + 8, "MertOS Grafik Testi Basarili!", ACCENT_CLR, BG_COLOR);

    cursor_y = by + bh + 8 + FONT_HEIGHT + 4;
    cursor_x = MARGIN_X;
}

static void cmd_time(void) {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    shell_print("\nCPU Tick: ", FG_COLOR);
    shell_print_hex(hi, TITLE_CLR);
    shell_print_hex(lo, TITLE_CLR);
    shell_print("\n", FG_COLOR);
}

static void process_command(void) {
    input_buffer[input_pos] = '\0';
    char *cmd = input_buffer;
    while (*cmd == ' ') cmd++;
    if (*cmd == '\0') return;

    char *args = cmd;
    while (*args && *args != ' ') args++;
    if (*args == ' ') { *args = '\0'; args++; while (*args == ' ') args++; }

    if (strcmp(cmd, "help") == 0) cmd_help();
    else if (strcmp(cmd, "clear") == 0) { fb_clear(BG_COLOR); cursor_x = MARGIN_X; cursor_y = MARGIN_Y; return; }
    else if (strcmp(cmd, "info") == 0) cmd_info();
    else if (strcmp(cmd, "reboot") == 0) cmd_reboot();
    else if (strcmp(cmd, "shutdown") == 0) cmd_shutdown();
    else if (strcmp(cmd, "echo") == 0) cmd_echo(args);
    else if (strcmp(cmd, "color") == 0) cmd_color();
    else if (strcmp(cmd, "logo") == 0) cmd_logo();
    else if (strcmp(cmd, "mem") == 0) cmd_mem();
    else if (strcmp(cmd, "gfx") == 0) cmd_gfx();
    else if (strcmp(cmd, "time") == 0) cmd_time();
    else {
        shell_print("\nBilinmeyen komut: ", ERR_CLR);
        shell_print(cmd, ERR_CLR);
        shell_print("\n'help' yazarak komutlari gorebilirsiniz.\n", FG_COLOR);
    }
}

void shell_init(void) {
    screen_w = fb_get_width();
    screen_h = fb_get_height();
    max_cols = (screen_w - 2 * MARGIN_X) / FONT_WIDTH;
    max_rows = (screen_h - 2 * MARGIN_Y) / FONT_HEIGHT;
    input_pos = 0;
    memset(input_buffer, 0, SHELL_BUFFER_SIZE);
}

static void print_welcome(void) {
    shell_print("  __  __           _    ___  ____  \n", TITLE_CLR);
    shell_print(" |  \\/  | ___ _ __| |_ / _ \\/ ___| \n", TITLE_CLR);
    shell_print(" | |\\/| |/ _ \\'__| __| | | \\___ \\ \n", TITLE_CLR);
    shell_print(" | |  | |  __/ |  | |_| |_| |___) |\n", TITLE_CLR);
    shell_print(" |_|  |_|\\___|_|   \\__|\\___/|____/ \n", TITLE_CLR);
    shell_print("\n", FG_COLOR);
    shell_print("  MertOS v2.0.0 - x86_64 Long Mode Isletim Sistemi\n", ACCENT_CLR);
    shell_print("  Gelistirici: Mert\n", PROMPT_CLR);
    shell_print("\n", FG_COLOR);
    shell_print("  Hosgeldiniz! 'help' yazarak komutlari gorebilirsiniz.\n", FG_COLOR);
    shell_print("  ==========================================================\n\n", 0x00444444);
}

void shell_run(void) {
    shell_init();
    print_welcome();
    print_prompt();
    draw_cursor(FG_COLOR);

    while (1) {
        char c = keyboard_getchar();
        draw_cursor(BG_COLOR);

        if (c == '\n') {
            shell_putchar('\n', FG_COLOR);
            process_command();
            input_pos = 0;
            memset(input_buffer, 0, SHELL_BUFFER_SIZE);
            print_prompt();
        } else if (c == '\b') {
            if (input_pos > 0) {
                input_pos--;
                input_buffer[input_pos] = '\0';
                cursor_x -= FONT_WIDTH;
                fb_fill_rect(cursor_x, cursor_y, FONT_WIDTH, FONT_HEIGHT, BG_COLOR);
            }
        } else if (input_pos < SHELL_BUFFER_SIZE - 1) {
            input_buffer[input_pos++] = c;
            shell_putchar(c, FG_COLOR);
        }

        draw_cursor(FG_COLOR);
    }
}

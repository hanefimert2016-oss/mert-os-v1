#include "vga.h"
#include "../lib/io.h"

static uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static uint8_t vga_row = 0;
static uint8_t vga_col = 0;
static uint8_t vga_color_attr = 0;

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

static uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
    return fg | bg << 4;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    vga_color_attr = vga_entry_color(fg, bg);
}

void vga_update_cursor(void) {
    uint16_t pos = vga_row * VGA_WIDTH + vga_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_init(void) {
    vga_color_attr = vga_entry_color(VGA_LIGHT_GREY, VGA_BLACK);
    vga_clear();
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', vga_color_attr);
    }
    vga_row = 0;
    vga_col = 0;
    vga_update_cursor();
}

void vga_scroll(void) {
    if (vga_row >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_entry(' ', vga_color_attr);
        }
        vga_row = VGA_HEIGHT - 1;
    }
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
    } else if (c == '\r') {
        vga_col = 0;
    } else if (c == '\t') {
        vga_col = (vga_col + 4) & ~3;
        if (vga_col >= VGA_WIDTH) {
            vga_col = 0;
            vga_row++;
        }
    } else {
        vga_buffer[vga_row * VGA_WIDTH + vga_col] = vga_entry(c, vga_color_attr);
        vga_col++;
        if (vga_col >= VGA_WIDTH) {
            vga_col = 0;
            vga_row++;
        }
    }
    vga_scroll();
    vga_update_cursor();
}

void vga_print(const char *str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_print_color(const char *str, uint8_t fg, uint8_t bg) {
    uint8_t old_color = vga_color_attr;
    vga_set_color(fg, bg);
    vga_print(str);
    vga_color_attr = old_color;
}

void vga_backspace(void) {
    if (vga_col > 0) {
        vga_col--;
    } else if (vga_row > 0) {
        vga_row--;
        vga_col = VGA_WIDTH - 1;
    }
    vga_buffer[vga_row * VGA_WIDTH + vga_col] = vga_entry(' ', vga_color_attr);
    vga_update_cursor();
}

void vga_print_dec(uint32_t num) {
    if (num == 0) {
        vga_putchar('0');
        return;
    }
    char buf[12];
    int i = 0;
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        vga_putchar(buf[j]);
    }
}

void vga_print_hex(uint32_t num) {
    vga_print("0x");
    char hex_chars[] = "0123456789ABCDEF";
    bool started = false;
    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (num >> i) & 0xF;
        if (nibble != 0 || started || i == 0) {
            vga_putchar(hex_chars[nibble]);
            started = true;
        }
    }
}

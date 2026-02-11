#include "idt.h"
#include "multiboot2.h"
#include "../drivers/framebuffer.h"
#include "../drivers/font.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"
#include "../lib/string.h"

#define BG_COLOR    0x001A1A2E
#define TITLE_CLR   0x0000DDFF
#define ACCENT_CLR  0x00FFD700
#define PROMPT_CLR  0x0000FF88
#define FG_COLOR    0x00E0E0E0

void kernel_main(uint32_t magic, uint32_t mb_info_addr) {
    idt_init();
    __asm__ volatile ("sti");

    struct multiboot2_tag_framebuffer *fb_tag = NULL;

    if (magic == MULTIBOOT2_MAGIC) {
        fb_tag = multiboot2_get_framebuffer(mb_info_addr);
    }

    if (fb_tag) {
        fb_init(fb_tag->framebuffer_addr, fb_tag->framebuffer_pitch,
                fb_tag->framebuffer_width, fb_tag->framebuffer_height,
                fb_tag->framebuffer_bpp);
    } else {
        fb_init(0xFD000000, 1024 * 4, 1024, 768, 32);
    }

    fb_clear(BG_COLOR);

    uint32_t w = fb_get_width();
    uint32_t h = fb_get_height();
    fb_fill_rect(0, 0, w, 3, TITLE_CLR);
    fb_fill_rect(0, h - 3, w, 3, TITLE_CLR);

    keyboard_init();
    shell_run();
}

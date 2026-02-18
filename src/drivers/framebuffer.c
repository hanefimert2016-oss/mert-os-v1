#include "framebuffer.h"
#include "../lib/string.h"

static struct framebuffer_info fb;

void fb_init(uint64_t addr, uint32_t pitch, uint32_t width, uint32_t height, uint8_t bpp) {
    fb.addr   = addr;
    fb.pitch  = pitch;
    fb.width  = width;
    fb.height = height;
    fb.bpp    = bpp;
}

void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= fb.width || y >= fb.height) return;
    uint32_t *pixel = (uint32_t *)(fb.addr + y * fb.pitch + x * (fb.bpp / 8));
    *pixel = color;
}

void fb_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t i = x; i < x + w && i < fb.width; i++) {
        fb_put_pixel(i, y, color);
        if (y + h - 1 < fb.height) fb_put_pixel(i, y + h - 1, color);
    }
    for (uint32_t j = y; j < y + h && j < fb.height; j++) {
        fb_put_pixel(x, j, color);
        if (x + w - 1 < fb.width) fb_put_pixel(x + w - 1, j, color);
    }
}

void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t j = y; j < y + h && j < fb.height; j++) {
        uint32_t *row = (uint32_t *)(fb.addr + j * fb.pitch + x * (fb.bpp / 8));
        for (uint32_t i = 0; i < w && (x + i) < fb.width; i++) {
            row[i] = color;
        }
    }
}

void fb_clear(uint32_t color) {
    fb_fill_rect(0, 0, fb.width, fb.height, color);
}

uint32_t fb_get_width(void) { return fb.width; }
uint32_t fb_get_height(void) { return fb.height; }
struct framebuffer_info *fb_get_info(void) { return &fb; }

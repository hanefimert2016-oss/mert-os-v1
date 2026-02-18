#ifndef FONT_H
#define FONT_H

#include "../lib/types.h"

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

void font_draw_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void font_draw_string(uint32_t x, uint32_t y, const char *str, uint32_t fg, uint32_t bg);

extern const uint8_t font8x16[128][16];

#endif

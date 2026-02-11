#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include "../lib/types.h"

#define MULTIBOOT2_MAGIC 0x36d76289

#define MULTIBOOT2_TAG_END          0
#define MULTIBOOT2_TAG_CMDLINE      1
#define MULTIBOOT2_TAG_BOOTLOADER   2
#define MULTIBOOT2_TAG_MEMORY       4
#define MULTIBOOT2_TAG_BOOTDEV      5
#define MULTIBOOT2_TAG_MMAP         6
#define MULTIBOOT2_TAG_FRAMEBUFFER  8

struct multiboot2_info {
    uint32_t total_size;
    uint32_t reserved;
};

struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot2_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
    uint8_t  red_field_position;
    uint8_t  red_mask_size;
    uint8_t  green_field_position;
    uint8_t  green_mask_size;
    uint8_t  blue_field_position;
    uint8_t  blue_mask_size;
};

struct multiboot2_tag_framebuffer *multiboot2_get_framebuffer(uint32_t addr);

#endif

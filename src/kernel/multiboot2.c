#include "multiboot2.h"

struct multiboot2_tag_framebuffer *multiboot2_get_framebuffer(uint32_t addr) {
    struct multiboot2_info *info = (struct multiboot2_info *)(uintptr_t)addr;
    struct multiboot2_tag *tag = (struct multiboot2_tag *)((uintptr_t)addr + 8);
    uintptr_t end = (uintptr_t)addr + info->total_size;

    while ((uintptr_t)tag < end && tag->type != MULTIBOOT2_TAG_END) {
        if (tag->type == MULTIBOOT2_TAG_FRAMEBUFFER) {
            return (struct multiboot2_tag_framebuffer *)tag;
        }
        uintptr_t next = (uintptr_t)tag + ((tag->size + 7) & ~7);
        tag = (struct multiboot2_tag *)next;
    }
    return NULL;
}

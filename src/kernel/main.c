/*
 * MorgOS Kernel - Main Entry Point
 * x86_64 minimal kernel with VGA output
 */

#include <stdint.h>
#include <stddef.h>

// VGA text mode buffer
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// VGA color codes
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

// Global variables
static uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0;

// External functions from ksys.c
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern void io_wait(void);

// Helper function to create VGA color
static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
    return fg | bg << 4;
}

// Helper function to create VGA entry
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// Initialize terminal
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Set terminal color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put character at specific position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = vga_entry(c, color);
}

// Put character at current position
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }
    
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

// Write string to terminal
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

// Write null-terminated string
void terminal_writestring(const char* data) {
    size_t len = 0;
    while (data[len]) {
        len++;
    }
    terminal_write(data, len);
}

// Simple printk function
void printk(const char* str) {
    terminal_writestring(str);
}

// GDT Entry structure
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// GDT Pointer structure
struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// GDT entries
struct gdt_entry gdt[5];
struct gdt_ptr gp;

// Set GDT gate
void gdt_set_gate(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

// Install GDT
void gdt_install(void) {
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (uint64_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF); // Code segment (64-bit)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xAF); // Data segment (64-bit)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xAF); // User code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xAF); // User data segment

    // Load GDT
    __asm__ volatile("lgdt %0" : : "m"(gp));
    
    // Reload segment registers
    __asm__ volatile(
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        : : : "ax"
    );
}

// IDT Entry structure
struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_mid;
    uint32_t base_hi;
    uint32_t reserved;
} __attribute__((packed));

// IDT Pointer structure
struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// IDT entries
struct idt_entry idt[256];
struct idt_ptr idtp;

// Set IDT gate
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_mid = (base >> 16) & 0xFFFF;
    idt[num].base_hi = (base >> 32) & 0xFFFFFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].reserved = 0;
}

// Install IDT
void idt_install(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint64_t)&idt;

    // Clear IDT
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idtp));
}

// Main kernel entry point
void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Set color to light green on black
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    
    // Print welcome message
    printk("MorgOS kernel alive\n");
    printk("Initializing...\n\n");
    
    // Initialize GDT
    printk("[*] Setting up GDT...\n");
    gdt_install();
    printk("[OK] GDT initialized\n");
    
    // Initialize IDT
    printk("[*] Setting up IDT...\n");
    idt_install();
    printk("[OK] IDT initialized\n");
    
    // Paging already enabled by bootloader
    printk("[OK] Paging enabled\n\n");
    
    // Final message
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    printk("Kernel initialized successfully!\n");
    printk("MorgOS v0.1 - Ready\n\n");
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    printk("System halted. No further tasks scheduled.\n");
    
    // Halt the CPU
    while (1) {
        __asm__ volatile("hlt");
    }
}

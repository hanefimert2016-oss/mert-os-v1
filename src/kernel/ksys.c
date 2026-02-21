/*
 * MorgOS Kernel System Functions
 * Basic I/O and helper functions
 */

#include <stdint.h>

// Output byte to I/O port
void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Input byte from I/O port
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Wait for I/O operation to complete
void io_wait(void) {
    // Port 0x80 is used for 'checkpoints' during POST
    // Writing to it causes a short delay
    outb(0x80, 0);
}

// Memory copy function
void* memcpy(void* dest, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (uint64_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

// Memory set function
void* memset(void* s, int c, uint64_t n) {
    uint8_t* p = (uint8_t*)s;
    for (uint64_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
    return s;
}

// String length
uint64_t strlen(const char* str) {
    uint64_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

// String compare
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const uint8_t*)s1 - *(const uint8_t*)s2;
}

// String copy
char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

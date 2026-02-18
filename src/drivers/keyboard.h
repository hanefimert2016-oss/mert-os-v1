#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../lib/types.h"

#define KEYBOARD_BUFFER_SIZE 256

void keyboard_init(void);
char keyboard_getchar(void);
bool keyboard_has_input(void);

#endif

#ifndef SHELL_H
#define SHELL_H

#include "../lib/types.h"

#define SHELL_BUFFER_SIZE 256
#define MAX_ARGS 16

void shell_init(void);
void shell_run(void);

#endif

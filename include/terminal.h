#ifndef TERMINAL_H
#define TERMINAL_H

#include "frame_buffer.h"
#include "io.h"
#include "keyboard.h"
#include "string.h"
#include "ascii_tables.h"

#define TERMINAL_Buffer_Size 70


char Terminal_Buffer[TERMINAL_Buffer_Size];
char Terminal_OUT_Buffer[70*40];

void write_terminal(char *Buffer, int len, int start, int x, int y);
void terminal_enter();

void terminal_renderer();
void terminal_console();
void terminal_handler();

#endif
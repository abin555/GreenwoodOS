#ifndef TERMINAL_H
#define TERMINAL_H

#include "frame_buffer.h"
#include "io.h"
#include "keyboard.h"
#include "string.h"
#include "ascii_tables.h"

#define TERMINAL_Buffer_Size 80
#define TERMINAL_SPLIT ' '

//#define Terminal_Y 20


char Terminal_Buffer[TERMINAL_Buffer_Size];
char Terminal_OUT_Buffer[TERMINAL_Buffer_Size*40];

char Terminal_Arguments[TERMINAL_Buffer_Size];

int terminal_compare(char *buffer, int start, int end, int len);

void terminal_interpret();
void terminal_output(char *Buffer, int start, int end);

void terminal_enter();

void terminal_renderer();
void terminal_console();
void terminal_handler();
#endif
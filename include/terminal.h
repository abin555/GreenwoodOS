#ifndef TERMINAL_H
#define TERMINAL_H

#include "frame_buffer.h"
#include "io.h"
#include "keyboard.h"
#include "string.h"
#include "ascii_tables.h"
#include "pong.h"
#include "grapher.h"
#include "memory.h"
#include "gcc_stdint.h"

#define TERMINAL_Buffer_Size 75
#define TERMINAL_SPLIT ' '

//#define Terminal_Y 20

char Terminal_Buffer[TERMINAL_Buffer_Size];
char Terminal_OUT_Buffer[TERMINAL_Buffer_Size*40];

char Terminal_Arguments[TERMINAL_Buffer_Size];

void terminal_memory_view();

int terminal_compare(char *buffer, int start, int end, int len);

void terminal_interpret();
void terminal_output(char *Buffer, int start, int end);

void terminal_enter();

void terminal_renderer();
void terminal_console();
void terminal_handler();

void terminal_init();
#endif
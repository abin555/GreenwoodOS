#ifndef TERMINAL_H
#define TERMINAL_H

#include "frame_buffer.h"
#include "io.h"
#include "keyboard.h"

#define TERMINAL_Buffer_Size 70

char Terminal_Buffer[TERMINAL_Buffer_Size];
char Terminal_Buffer_Pointer;
char Terminal_OUT_Buffer[70*40];
unsigned short Terminal_OUT_pointer;

void terminal_renderer();

void terminal_handler();

#endif
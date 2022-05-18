#ifndef CONSOLE_H
#define CONSOLE_H

#include "frame_buffer.h"
#include "memory.h"
#include "string.h"

char *consoleArray;
uint32_t consoleLine;
uint32_t consoleStart;
uint32_t consoleSize;

void initializeConsole();
void console_putscreen();
void kprint(char* msg);
void kprintF(const char* restrict fmt, ...);
void panic(const char* restrict msg);

#endif
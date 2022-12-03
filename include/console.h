#ifndef CONSOLE_H
#define CONSOLE_H

#include "stdint.h"
#include "memory.h"
#include <stdarg.h>

char* consoleArray;
uint32_t consoleLine;
uint32_t consoleStart;
uint32_t consoleSize;
uint32_t consoleLinePlace;
uint32_t console_width;
uint32_t console_height;
uint32_t console_last_line;
char console_initialized;
uint32_t console_color_fg;
uint32_t console_color_bg;

void initialize_console(uint32_t width, uint32_t height);

void console_putScreen();
void console_putLine(uint32_t places);
void shiftConsole();
int calculateNumberLength(unsigned int data, int base);
int printDecimal(unsigned int data, int setlength);
int printHex(unsigned int data, int setlength);
int printBinary(unsigned int data, int setlength);
void printk(char* msg, ...);
char quadToHex(char quad);

void console_clear();

#endif
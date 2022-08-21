#ifndef CONSOLE_H
#define CONSOLE_H

#include "frame_buffer.h"
#include "memory.h"
#include "string.h"
#include <stdarg.h>

char *consoleArray;
uint32_t consoleLine;
uint32_t consoleStart;
uint32_t consoleSize;
uint32_t consoleLinePlace;

void initializeConsole();
void shiftConsoleUp();
int calculateNumberLength(unsigned int data, int base);
int printDecimal(unsigned int data, int setlength);
int printHex(unsigned int data, int setlength);
int printBinary(unsigned int data, int setlength);

void console_putscreen();
void printk(char* msg, ...);
void kprintF(const char* restrict fmt, ...);
void panic(const char* restrict msg);
void console_moveline(int count);
#endif
#ifndef TERMINAL_H
#define TERMINAL_H

#include "keyboard.h"
#include "memory.h"
#include "console.h"
#include "interrupts.h"
#include "timer.h"
#include "filesystem.h"
#include "processes.h"
#include "programs/editor.h"
#include "programs/art.h"
#include "programs/hex.h"
#include "programs.h"
#include "ISO9660.h"
#include "utilities.h"

char* terminal_buffer;
char* terminal_arg_buffer;

uint32_t terminal_buffer_index;
uint32_t terminal_last_key;
uint32_t terminal_last_char;

uint16_t terminal_number_of_blocks;
uint16_t terminal_block_index[0xFF];

int strcmp(const char *X, const char *Y, int block);

void init_terminal();

void terminal_locate_blocks();
void terminal_parse();

void terminal_callback(uint8_t process_id, uint32_t args[10]);

#endif
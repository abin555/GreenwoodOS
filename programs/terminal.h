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

char* terminal_buffer;
uint32_t terminal_buffer_index;
uint32_t terminal_last_key;
uint32_t terminal_last_char;

void init_terminal();

void terminal_callback(uint8_t process_id);

#endif
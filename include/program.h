#ifndef PROGRAM_H
#define PROGRAM_H

#define PROGRAM_MAX 10
#define PROGAM_MAX_SIZE 0x400000
#define PROGRAM_VIRT_REGION_BASE 0x01000000

#include "multitasking.h"
#include "drive.h"
#include "memory.h"

extern char programs[PROGRAM_MAX];

void program_init();
void select_program(uint8_t program_slot);
void exec(char *filename);

#endif
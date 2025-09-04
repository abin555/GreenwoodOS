#ifndef PROGRAM_H
#define PROGRAM_H
#include "stdbool.h"
#include "multitasking.h"
#include "drive.h"
#include "memory.h"

#define PROGRAM_MAX 30
#define PROGAM_MAX_SIZE 0x400000
#define PROGRAM_VIRT_REGION_BASE 0x0B000000

extern bool program_slot_status[PROGRAM_MAX];
extern uint8_t program_active_slot;

void program_init();
void select_program(int program_slot);
int exec(char *filename, int argc, char **argv);

#endif
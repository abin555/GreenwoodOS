#ifndef PROGRAMS_H
#define PROGRAMS_H

#include "processes.h"
#include "paging.h"
#include "io.h"
#include "filesystem.h"

extern void program();
extern void program_end();
extern uint8_t program_active_slot;
void exec_user_program(int pid, uint32_t args[10]);
void init_program_memory();
void select_program(uint8_t program_slot);

struct FS_Item_Entry;

void load_program(uint8_t program_slot, struct FS_Item_Entry* file);

int reserveProgramSlot();
void freeProgramSlot(int slot);

#endif
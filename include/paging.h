#ifndef PAGING_H
#define PAGING_H

#include "stdint.h"
#include "memory.h"
#include "io.h"
#include "interrupts.h"
#include "console.h"
#include "multiboot.h"
#include "programs.h"

extern void update_page();
extern uint32_t boot_page_directory; 

uint32_t get_page_index_from_addr(uint32_t address); 


void init_paging();

void paging_error();

#endif
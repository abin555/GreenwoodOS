#ifndef PAGING_H
#define PAGING_H

#include "stdint.h"
#include "memory.h"
#include "io.h"
#include "interrupts.h"
#include "console.h"
#include "multiboot.h"
#include "programs.h"
#include "cpu.h"
#include "pcspk.h"

extern void update_page();
extern uint32_t boot_page_directory; 

uint32_t get_page_index_from_addr(uint32_t address); 

void init_paging();

void paging_error();

uint32_t *page_directory;
extern void load_page_directory();
extern void enable_paging();
void create_page_entry(
    uint32_t base_address,
    uint32_t target_address,
    uint16_t flag
);

static inline void __native_flush_tlb_single(unsigned long addr) {
   asm volatile("invlpg [%0]" ::"r" (addr) : "memory");
}

uint32_t get_physical(uint32_t address);

void set_PAT();

void dump_page_map();
#endif
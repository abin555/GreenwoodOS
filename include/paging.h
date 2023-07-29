#ifndef PAGING_H
#define PAGING_H

#include "stdint.h"
#include "serial.h"
#include "io.h"
#include "cpu.h"

#define PAGE_SIZE 0x400000

extern void update_page();
extern uint32_t boot_page_directory; 
uint32_t get_page_index_from_addr(uint32_t address); 

void page_init();

void page_error();
void create_page_entry(
	uint32_t base_address, 
	uint32_t target_address,
	 uint16_t flag);

static inline void __native_flush_tlb_single(unsigned long addr) {
   asm volatile("invlpg [%0]" ::"r" (addr) : "memory");
}

uint32_t get_physical(uint32_t address);
uint32_t get_virtual(uint32_t address);

void set_PAT();

#endif
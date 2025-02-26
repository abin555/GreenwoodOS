#include "allocator.h"


void *alloc_base;
const uint32_t alloc_table_size = 0xB00000;
uint32_t total_alloc = 0;
uint8_t first_alloc = 1;

void alloc_init(){
	MEM_printRegions();
	//printk("Inside Alloc\n");
	print_serial("[ALLOC] Init\n");
	int region_idx = MEM_findRegionIdx(alloc_table_size);
	alloc_base = (void *) 0x90000000;
	MEM_reserveRegionBlock(region_idx, alloc_table_size, (uint32_t) alloc_base, ALLOC);
	MEM_printRegions();
}

void *malloc(uint32_t size){
	size += 10;
	void *result = alloc_base + total_alloc;
	total_alloc += size;
	return result;
}
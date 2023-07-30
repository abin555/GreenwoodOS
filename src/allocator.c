#include "allocator.h"

void *alloc_base;
const uint32_t alloc_table_size = 0x800000;
uint32_t total_alloc = 0;
uint8_t first_alloc = 1;

void alloc_init(){
	int region_idx = MEM_findRegionIdx(alloc_table_size);
	alloc_base = (void *) 0xA0000000;
	MEM_reserveRegionBlock(region_idx, alloc_table_size, (uint32_t) alloc_base, ALLOC);
}

void *malloc(uint32_t size){
	struct ALLOC_TABLE *alloc_table = (struct ALLOC_TABLE *) alloc_base;
	if(first_alloc){
		alloc_table->size = size;
		alloc_table->next = NULL;
		alloc_table->prev = NULL;
		alloc_table->used = 1;
		first_alloc = 0;
		return (void *) alloc_table + sizeof(struct ALLOC_TABLE);
	}


}
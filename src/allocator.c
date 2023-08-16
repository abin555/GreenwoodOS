#include "allocator.h"


void *alloc_base;
const uint32_t alloc_table_size = 0x800000;
uint32_t total_alloc = 0;
uint8_t first_alloc = 1;

void alloc_init(){
	//printk("Inside Alloc\n");
	int region_idx = MEM_findRegionIdx(alloc_table_size);
	alloc_base = (void *) 0x90000000;
	MEM_reserveRegionBlock(region_idx, alloc_table_size, (uint32_t) alloc_base, ALLOC);
}

void *malloc(uint32_t size){
	size += 10;
	//print_serial("[MALLOC] Alloc for size %x Total Remaining: %x\n", size, alloc_table_size - total_alloc);
	struct ALLOC_TABLE *alloc_table = (struct ALLOC_TABLE *) alloc_base;
	//Building initial head block

	if(total_alloc + size >= alloc_table_size){
		print_serial("ERROR: Alloc Table Size is larger than available!\n");
		return NULL;
	}

	if(first_alloc){
		alloc_table->size = size;
		alloc_table->next = NULL;
		alloc_table->prev = NULL;
		alloc_table->used = 1;
		first_alloc = 0;
		total_alloc += alloc_table->size + sizeof(struct ALLOC_TABLE);
		return (void *) alloc_table + sizeof(struct ALLOC_TABLE);
	}
	//Going through previously occupied blocks.
	/*
	while(alloc_table->next != NULL){
		if(!alloc_table->next->used && alloc_table->next->size <= size){
			alloc_table->next->used = 1;
			total_alloc += alloc_table->size + sizeof(struct ALLOC_TABLE);
			return (void *) alloc_table + sizeof(struct ALLOC_TABLE);
		}
		alloc_table = alloc_table->next;
	}
	*/
	//New Block Region
	//struct ALLOC_TABLE *new_alloc_table = (struct ALLOC_TABLE *) ((uint32_t) alloc_table + alloc_table->size + sizeof(struct ALLOC_TABLE));
	struct ALLOC_TABLE *new_alloc_table = (struct ALLOC_TABLE *) ((uint32_t) alloc_table + total_alloc);
	if(
		(uint32_t) new_alloc_table >= ((uint32_t) alloc_base + alloc_table_size) 
		|| ((uint32_t) new_alloc_table + size + sizeof(struct ALLOC_TABLE)) >= ((uint32_t) alloc_base + alloc_table_size)
	){
		return NULL;
	}
	alloc_table->next = new_alloc_table;
	new_alloc_table->size = size;
	new_alloc_table->used = 1;
	new_alloc_table->prev = alloc_table;
	new_alloc_table->next = NULL;
	total_alloc += new_alloc_table->size + sizeof(struct ALLOC_TABLE);
	return (void *) (new_alloc_table + sizeof(struct ALLOC_TABLE));
}
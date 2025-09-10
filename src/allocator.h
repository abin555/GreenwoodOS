#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "memory.h"
#include "stdint.h"
#include "framebuffer.h"
#include "paging.h"
#include "serial.h"

struct ALLOC_TABLE{
	uint32_t size;//Not including table header...
	struct ALLOC_TABLE *next;//Points to header of next block, NULL if end
	struct ALLOC_TABLE *prev;//Points to header of previous block, NULL if beginning
	uint8_t used;
};

extern const uint32_t alloc_table_size;
extern uint32_t total_alloc;

void alloc_init();

void *malloc(uint32_t size);
void free(void *block);

#endif
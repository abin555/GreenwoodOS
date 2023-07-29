#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "memory.h"
#include "stdint.h"
#include "framebuffer.h"
#include "paging.h"
#include "serial.h"

struct ALLOC_TABLE{
	uint32_t size;//Not including table header...
	void *next;//Points to header of next block, NULL if end
	void *prev;//Points to header of previous block, NULL if beginning
};

void *malloc(uint32_t size);
void free(void *block);

#endif
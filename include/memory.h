#ifndef MEMORY_H
#define MEMORY_H

#include "console.h"
#include "stdint.h"
#include "framebuffer.h"
#include "paging.h"
#include "serial.h"

#define HEAP_NUMBER_OF_BLOCKS 0x1000

uint32_t MEMORY_USED;
uint32_t HEAP_BEGIN;
uint32_t HEAP_END;

typedef struct alloc_t{
    uint32_t size;
    uint8_t status;
} alloc_t;

uint8_t HEAP_ALLOC_TABLE[HEAP_NUMBER_OF_BLOCKS / 8];

void memcpy(void* source, void* target, uint32_t size);

void memset(void* target, int value, uint32_t size);
unsigned int mgetSize(void *mem);

void* malloc(unsigned int size);
void free(void *memory);
void initialize_heap(uint32_t heap_begin, uint32_t heap_size);
void mem_dump();
#endif
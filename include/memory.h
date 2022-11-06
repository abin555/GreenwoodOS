#ifndef MEMORY_H
#define MEMORY_H

#include "console.h"
#include "stdint.h"
#include "framebuffer.h"

#define HEAP_SIZE 0x100000

uint32_t MEMORY_USED;
uint32_t HEAP_BEGIN;
uint32_t HEAP_END;

typedef struct alloc_t{
    uint32_t size;
    uint8_t status;
} alloc_t;

void memcpy(void* source, void* target, uint32_t size);
void memset(void* target, uint32_t val, uint32_t size);
unsigned int mgetSize(void *mem);

void* malloc(unsigned int size);
void free(void *memory);
void initialize_heap(uint32_t heap_begin, uint32_t heap_size);

#endif
#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include "gcc_stdint.h"
#include "frame_buffer.h"
#include "string.h"

uint32_t memory_used;
uint32_t heap_begin;
uint32_t heap_end;

typedef struct{
    uint32_t size;
    uint8_t status;//
} alloc_t;

void memcpy(u64* source, u64* target, u64 len);
void* memset(void * place, int val, unsigned int size);

void* malloc(unsigned int size);
void free(void *mem);
void mem_init(uint32_t kernelEnd);
unsigned int mgetSize(void *mem);
#endif
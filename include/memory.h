#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include "gcc_stdint.h"


typedef struct {
    uint8_t status;
    uint8_t size;
} alloc_t;

void memcpy(u64* source, u64* target, u64 len);
void* memset(void * place, int val, unsigned int size);

char* malloc(unsigned int size);
void mem_init(uint32_t kernelEnd);
#endif
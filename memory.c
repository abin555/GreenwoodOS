#include "memory.h"

#define HEAP_SIZE 0x00400000 //Heap is 4 MB in size

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t memory_used = 0;

void memcpy(u64* source, u64* target, u64 len){
    for(u64 index = 0; index < len; index++){
        *(volatile u64*)(target+index) = *(volatile u64*)(source+index);
    }
}

void* memset(void * place, int val, unsigned int size){
    unsigned char* p = place;
    while(size--){
        *p++ = (unsigned char) val;
    }
    return place;
}

void mem_init(uint32_t kernelEnd){
    last_alloc = kernelEnd + 0x1000;
    heap_begin = last_alloc;
    heap_end = heap_begin + HEAP_SIZE;
    memset((char *) heap_begin, 0, heap_end - heap_begin);
}

char* malloc(unsigned int size){
    if(!size) return 0;

    uint8_t *mem = (uint8_t *)heap_begin;

    while((uint32_t) mem < last_alloc){
        alloc_t *a = (alloc_t *) mem;

        if(!a->size)
            goto nalloc;
        
        if(a->status){
            mem += a->size;
            mem += sizeof(alloc_t);
            mem += 4;
            continue;
        }

        if(a->size >= size){
            a->status = 1;

            memset(mem + sizeof(alloc_t), 0, size);
            memory_used += size + sizeof(alloc_t);
            return (char *)(mem + sizeof(alloc_t));
        }

        mem += a->size;
        mem += sizeof(alloc_t);
        mem += 4;
    }

    nalloc:;
    return 0;//TODO CONTINUE WORK
}
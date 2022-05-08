# 1 "memory.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "memory.c"
# 1 "./include/memory.h" 1



# 1 "./include/types.h" 1
# 21 "./include/types.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long int i64;
# 5 "./include/memory.h" 2
# 1 "./include/gcc_stdint.h" 1
# 34 "./include/gcc_stdint.h"
typedef signed char int8_t;


typedef short int int16_t;


typedef int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef int intptr_t;


typedef unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 6 "./include/memory.h" 2


typedef struct {
    uint8_t status;
    uint8_t size;
} alloc_t;

void memcpy(u64* source, u64* target, u64 len);
void* memset(void * place, int val, unsigned int size);

char* malloc(unsigned int size);
void mem_init(uint32_t kernelEnd);
# 2 "memory.c" 2



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
    heap_end = heap_begin + 0x00400000;
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
    return 0;
}

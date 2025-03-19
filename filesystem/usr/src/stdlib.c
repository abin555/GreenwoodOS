#include <stdlib.h>
#include <stdio.h>
#include <sys/memory.h>

struct STDLIB_Memory {
    void *region_base;
    int region_size;
    void *region_head;
};

struct STDLIB_Memory memory;

struct Heap {
    void *head;
};

struct Heap heap;


void init_stdlib(){
    memory.region_size = 0x00400000*10;
    memory.region_base = memory_requestRegion(memory.region_size);
    memory.region_head = memory.region_base;
    heap.head = memory.region_head;
}

void clean_stdlib(){
    memory_returnRegion(memory.region_base, memory.region_size);
}

void *malloc(size_t size){
    void *addr = heap.head;
    heap.head += size;
    return addr;
}

void *calloc(size_t nmemb, size_t size){
	return malloc(nmemb * size);
}

#include <string.h>
void *realloc(void *ptr, size_t size){
	void *new_alloc = malloc(size);
	memcpy(new_alloc, ptr, size);
	return new_alloc;
}
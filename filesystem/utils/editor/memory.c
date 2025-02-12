#include "memory.h"

struct MemoryTable {
    void *memregion;
    uint32_t regionSize;
    void *heap_top;
} memTable;

void memsetup(uint32_t regionSize){
    memTable.regionSize = regionSize;
    memTable.memregion = requestRegion(memTable.regionSize);
    memTable.heap_top = memTable.memregion;
}

void *malloc(uint32_t size){
    if(memTable.heap_top + size > memTable.memregion + memTable.regionSize) return NULL;
    void *ptr = memTable.heap_top;
    memTable.heap_top += size;
    return ptr;
}

void memfinish(){
    freeRegion(memTable.memregion, memTable.regionSize);
    memTable.heap_top = NULL;
    memTable.memregion = NULL;
    memTable.regionSize = 0;
}

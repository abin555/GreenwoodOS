#include "memory.h"

uint32_t last_alloc = 0;
uint32_t HEAP_BEGIN = 0;
uint32_t HEAP_END = 0;
uint32_t MEMORY_USED = 0;

void memcpy(void* source, void* target, uint32_t size){
    if((uint32_t) source > (uint32_t) target){
        for(uint32_t index = 0; index < size; index++){
            *(volatile char*)(target+index) = *(volatile char*)(source+index);
        }
    }
    else{
        for(uint32_t index = size; index > 0; index--){
            *(volatile char*)(target+index) = *(volatile char*)(source+index);
        }
    }
}

void memset(void* target, uint32_t val, uint32_t size){
    unsigned char* place = target;
    while(size--){
        *place++ = (unsigned char) val;
    }
}

void* malloc(unsigned int size){
    if(!size) return 0;
    
    uint32_t mem = (uint32_t) HEAP_BEGIN;

    while((uint32_t) mem < HEAP_END){
        alloc_t *a = (alloc_t *) mem;

        if(!a->size && !a->status){//If there is not an allocation at this location, goto the not allocated label
            goto notallocated;
        }
        if(a->status){//If status is defined, the allocation segment is in use.
            mem += a->size;//move up to the size of the segment
            mem += sizeof(alloc_t);//add the size of the info table
            //mem += 4;//add four?
            continue; //move on to next try
        }
        
        //In order to get to this point, there need to be an existing allocation region defined at *mem
        //However, it cannot have the status flag enabled.
        //Based on this, when a region is set and the region is not used, the region is reallocated.
        
        if(a->size == size){//Check if the available allocation region contains 
            a->status = 1;//mark region as in use.;
            memset((uint32_t *) mem + sizeof(alloc_t), 0, size); //clear requested & available region.
            MEMORY_USED += size; //Increase counter of used memory
            //printHeap();
            return (void *)((uint32_t) mem + sizeof(alloc_t));
            //last_alloc = (uint32_t) mem;
        } 
        mem += a->size + sizeof(alloc_t);
        continue;
    }
    notallocated:;//Label for unallocated memory

    if(last_alloc+size+sizeof(alloc_t) >= HEAP_END){//Check if the location of the last allocated block plus the size requested would be past the heap end
        char error[] = "Out of HEAP memory";
        fb_write_xy(error, sizeof(error), 0, 0, 0xFFFFFF, 0);
        return 0;
    }

    alloc_t *lalloc = (alloc_t *) last_alloc;//get data from last allocation
    alloc_t *alloc = (alloc_t *) last_alloc + ((lalloc->status) ? (lalloc->size + sizeof(alloc_t)) : 0);//create new allocation pointer based on last alloc
    alloc->status = 1;//Set alloc to be used
    alloc->size = size;//set size of allocation

    //increase the location of the last alloc
    last_alloc += size;
    last_alloc += sizeof(alloc_t);
    //last_alloc += 4;
    //increase memory labeled as used
    MEMORY_USED += size + sizeof(alloc_t);

    //set memory in block
	memset((void *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);

    //printHeap();
    //return pointer
	return (void *)((uint32_t)alloc + sizeof(alloc_t));
}

void free(void* memory){
    alloc_t* alloc = (memory - sizeof(alloc_t));
    MEMORY_USED -= alloc->size;

    alloc->status = 0;
    if((uint32_t) memory == last_alloc){
        last_alloc -= (uint32_t) memory + sizeof(alloc_t);
    }
}

unsigned int mgetSize(void *mem){
    alloc_t* alloc = (alloc_t *) (mem - sizeof(alloc_t));
    return alloc->size;
}

void initialize_heap(uint32_t heap_begin, uint32_t heap_size){
    last_alloc = heap_begin;
    HEAP_BEGIN = heap_begin;
    HEAP_END = HEAP_BEGIN + heap_size;
    memset((char *) HEAP_BEGIN, 0, heap_size);
    fb_write_xy("HEAP READY", 10, 0, 0, 0xFFFFFF, 0);
}
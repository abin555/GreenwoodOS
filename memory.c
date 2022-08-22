#include "memory.h"

#define HEAP_SIZE 0x100000 //Heap is 4 MB in size

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
    last_alloc = kernelEnd;
    heap_begin = last_alloc;
    heap_end = heap_begin + HEAP_SIZE;
    memset((char *) heap_begin, 0, HEAP_SIZE);
    placement_address = heap_begin + HEAP_SIZE + 0x1000;
}



void* malloc(unsigned int size){
    if(!size) return 0;
    
    uint32_t mem = (uint32_t) heap_begin;

    while((uint32_t) mem < heap_end){
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
            memory_used += size; //Increase counter of used memory
            //printHeap();
            return (void *)((uint32_t) mem + sizeof(alloc_t));
            //last_alloc = (uint32_t) mem;
        } 
        mem += a->size + sizeof(alloc_t);
        continue;
    }
    notallocated:;//Label for unallocated memory

    if(last_alloc+size+sizeof(alloc_t) >= heap_end){//Check if the location of the last allocated block plus the size requested would be past the heap end
        char error[] = "Out of HEAP memory";
        fb_write_xy(error, sizeof(error), 0, 0, 0);
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
    memory_used += size + sizeof(alloc_t);

    //set memory in block
	memset((void *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);

    //printHeap();
    //return pointer
	return (void *)((uint32_t)alloc + sizeof(alloc_t));
}

void free(void *mem){
    alloc_t *alloc = (mem - sizeof(alloc_t));
    memory_used -= alloc->size;
    //memset(mem, 0, alloc->size + sizeof(alloc_t));
    alloc->status = 0;
    if((uint32_t) mem == last_alloc){
        last_alloc -= (uint32_t) mem + sizeof(alloc_t);
    }
    //printHeap();
}

unsigned int mgetSize(void *mem){
    alloc_t *alloc = (alloc_t *) (mem - sizeof(alloc_t));
    return alloc->size;
}

void printHeap(){
    char workspace[] = "_____MEM | _Address | ____Size | __Status";
    fb_set_color(0x03fc1c, 0);
    fb_write_xy(workspace, sizeof(workspace), 0, fb_terminal_w-sizeof(workspace), 0);
    fb_set_color(0xFFFFFF, 0);
    strcpy("_________________________________________", workspace, sizeof(workspace));
    fb_write_xy(workspace, sizeof(workspace), 0, fb_terminal_w-sizeof(workspace), 1);
    strcpy("                                         ", workspace, sizeof(workspace));

    uint32_t mem = (uint32_t ) heap_begin;
    //uint32_t nextaddr = (uint32_t) mem;
    uint32_t layer = 2;
    while(mem < last_alloc){
        alloc_t *a = (alloc_t *) mem;
        decodeHex(workspace, mem, 32, -1);
        decodeHex(workspace, mem + sizeof(alloc_t), 32, 10);
        decodeHex(workspace, a->size, 32, 21);
        decodeHex(workspace, a->status, 32, 32);
        fb_write_xy(workspace, sizeof(workspace), 0, fb_terminal_w-sizeof(workspace), layer);
        layer++;
        mem = mem +  a->size + sizeof(alloc_t);

    }
}

uint32_t placement_address;
uint32_t kmalloc(uint32_t size, int align, uint32_t* physical_mem){
    if (align == 1 && (placement_address & 0xFFFFF000)){ // If the address is not already page-aligned
        // Align it.
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (physical_mem){
        *physical_mem = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}
#ifndef PAGING_H
#define PAGING_H

#include "memory.h"
#include "interrupts.h"
#include "console.h"
#include "stdint.h"

struct page{
    uint32_t present    : 1; //Page is present in memory
    uint32_t rw         : 1; //Read only if clear, read write if set
    uint32_t user       : 1; //Supervisor level only if clear
    uint32_t accessed   : 1; //Has the page been accesssed since last refresh?
    uint32_t dirty      : 1; //Has the page been writen to since last refresh?
    uint32_t unused     : 1; //Unused and reserved bits
    uint32_t frame      : 20;//Frame Address
};

struct page_table{
    struct page pages[1024];
};

struct page_directory{
    //Array of pointers to pagetables
    struct page_table* tables[1024];
    //The physical location of the memory to be loaded into CR3
    uint32_t tablesPhysical[1024];
    //Physical adress of tables Physical for when the kernel heap is allocated and the dir can be in virtual memory
    uint32_t physicalAddr;
};
extern struct page_directory* current_directory;
//Sets up the env, page dirs, and enables paging
void init_paging();
//Cause page directory to be loaded into CR3 reg
void switch_page_directory(struct page_directory* page_dir);
//Retrieves a pointer to the page required
struct page* get_page(uint32_t address, int make, struct page_directory* dir);

//Handler for page faults
void page_fault(struct cpu_state cpu);

#endif
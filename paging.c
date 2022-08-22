#include "paging.h"

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

uint32_t* frames;
uint32_t nframes;


//Set a bit in the frames bitset
static void set_frame(uint32_t frame_addr){
    uint32_t frame = frame_addr/0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}
//Clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr){
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}
/*
//Test if a bit is set
static uint32_t test_frame(uint32_t frame_addr){
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}*/

//Find the first free frame
static uint32_t first_frame(){
    uint32_t i, j;
    for(i = 0; i < INDEX_FROM_BIT(nframes); i++){
        if(frames[i] != 0xFFFFFFF){//Nothing free, exit
            for(j = 0; j < 32; j++){
                uint32_t toTest = 0x1 << j;
                if(!(frames[i]&toTest)){
                    return i*4*8+j;
                }
            }
        }
    }
    return 0;
}

void alloc_frame(struct page* page, int is_kernel, int is_writeable){
    if(page->frame != 0){
        return; // Frame is already allocated, return
    }
    else{
        uint32_t idx = first_frame();
        if(idx == (uint32_t) -1){
            panic("[PAGING] No free frames!");
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = idx;
    }
}

void free_frame(struct page* page){
    uint32_t frame;
    if(!(frame=page->frame)){
        return; //It actually wasnt allocated at all
    }
    else{
        clear_frame(frame);
        page->frame = 0x0;
    }
}
struct page_directory* current_directory;
void init_paging(){
    printk("[PAGING] Initializing\n");
    uint32_t mem_end_page = 0x10000000;
    nframes = mem_end_page / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes), 0, 0);
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    struct page_directory* kernel_directory = (struct page_directory*) kmalloc(sizeof(struct page_directory), 0, 0);
    memset(kernel_directory, 0, sizeof(struct page_directory));
    current_directory = kernel_directory;

    uint32_t i = 0;
    while(i < placement_address){
        //Kernel is readable but not writeable from userspace
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    //register page fault handler
    interrupt_add_handle(14, &page_fault);

    //Enable paging!
    switch_page_directory(kernel_directory);
    printk("[PAGING] Done");
}

void switch_page_directory(struct page_directory* dir){
    current_directory = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

struct page* get_page(uint32_t address, int make, struct page_directory* dir){
    address /= 0x1000;

    uint32_t table_idx = address / 1024;
    if(dir->tables[table_idx]){
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    else if(make){
        uint32_t tmp;
        dir->tables[table_idx] = (struct page_table*) kmalloc(sizeof(struct page_table), 1, &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7;
        return &dir->tables[table_idx]->pages[address%1024];
    }
    return 0;
}

void page_fault(struct cpu_state cpu __attribute__((unused))){__attribute__((unused))
    //Page fault handler
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

    struct stack_state regs = most_recent_int_stack_state;
    int present   = !(regs.error_code & 0x1); // Page not present
    int rw = regs.error_code & 0x2;           // Write operation?
    int us = regs.error_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.error_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.error_code & 0x10;          // Caused by an instruction fetch?

    printk("Page fault! ( ");
    if(present) printk("present ");
    if(rw) printk("read-only ");
    if(us) printk("user-mode ");
    if(reserved) printk("reserved ");
    if(id) printk("instr fetch ");
    printk(") at 0x");
    printk("%8x\n", faulting_address);
    panic("PAGE FAULT");
}
#include "paging.h"
#include "serial.h"

uint32_t get_page_index_from_addr(uint32_t address){
    return (address >> 22);
}

void init_paging(){
    printk("[Paging] Begin Init\n");
    interrupt_add_handle(0xE, paging_error);

    printk("[Paging] Page Table Addr: 0x%x\n", &boot_page_directory);
    uint32_t *page_table = (uint32_t*) &boot_page_directory;
    page_table[0] = ((uint32_t) program & 0xFFFF0000) | 0x83;
    //page_table[get_page_index_from_addr(0xffffb6b2)] = (0xffffb6b2 & 0xFFFF0000) | 0x83;
    for(int i = 0; i < 1024; i++){
        if(page_table[i]){
            printk("[Page Table] index %x = %x\n", i, page_table[i]);
        }
    }
    printk("%x\n", ((uint32_t) program & 0xFFFF0000) | 0x83);
    update_page();
    printk("[Paging] Initialized\n");
}

void paging_error(){
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    printk("Page Fault at 0x%x\n", faulting_address);
    print_serial("Page Fault at 0x%x\n", faulting_address);
    printk("[KERNEL PANIC]\n");
    asm volatile("cli");
    asm volatile("hlt");
}
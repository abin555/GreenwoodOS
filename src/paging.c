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
    //page_table[0] = ((uint32_t) program & 0xFFFF0000) | 0x83;
    //page_table[get_page_index_from_addr(0xffffb6b2)] = (0xffffb6b2 & 0xFFFF0000) | 0x83;
    create_page_entry((uint32_t) program, 0x0, 0x83);
    create_page_entry((uint32_t) real_framebuffer, (uint32_t) real_framebuffer, 0x93);
    create_page_entry((uint32_t) real_framebuffer+0x400000, (uint32_t) real_framebuffer+0x400000, 0x93);
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
    console_initialized = 1;
    console_fullPut();
    printk("Page Fault at 0x%x\n", faulting_address);
    print_serial("Page Fault at 0x%x\n", faulting_address);
    printk("[KERNEL PANIC]\n");
    play_sound(1000);
    for(uint32_t i = 0; i < (0xFFFFFFE / 2); i++){}
    mute();
    asm volatile("cli");
    asm volatile("hlt");
}

void create_page_entry(
    uint32_t base_address,
    uint32_t target_address,
    uint16_t flag
){
    uint32_t *page_table = (uint32_t*) &boot_page_directory;
    page_table[get_page_index_from_addr(target_address)] = ((base_address & 0xFFC00000) | flag);
    printk("[Paging] Mapped Addr: %x at Index: %x To Addr: %x Set Value: %x\n", base_address, get_page_index_from_addr(target_address), target_address, ((base_address & 0xFFC00000) | 0x83));
    __native_flush_tlb_single(target_address);
}

uint32_t get_physical(uint32_t address){
    uint32_t lower_addr = address & ~0xFFC00000;
    uint32_t* page_table = (uint32_t *) &boot_page_directory;
    uint32_t page_dir = address >> 22;
    uint32_t value = page_table[page_dir];
    value &= 0xFFC00000;
    value |= lower_addr;
    return value;
}

void set_PAT(){
    if(getCPUFeatures(CPUID_FEAT_EDX_PAT)){
        uint32_t IA32_PAT_low = getMTRR_low(0x277);
        IA32_PAT_low = 0b00000000000000010000010000000110;
        setMTRR_low(0x277, IA32_PAT_low);
        IA32_PAT_low = getMTRR_low(0x277);
    }
    else{
        printk("Unable to set PAT");
    }
}
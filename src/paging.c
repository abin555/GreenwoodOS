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
    create_page_entry((uint32_t) real_framebuffer, (uint32_t) real_framebuffer, 0x93);
    create_page_entry((uint32_t) real_framebuffer+0x400000, (uint32_t) real_framebuffer+0x400000, 0x93);

    for(int i = 0; i < 1024; i++){
        if(page_table[i]){
            printk("[Page Table] index %x = %x\n", i, page_table[i]);
        }
    }
    update_page();
    printk("[Paging] Initialized\n");
}

void paging_error(){
    asm volatile("cli");
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    print_serial("Page Fault at 0x%x\n", faulting_address);
    console_initialized = 1;
    fb_clear(0x1111FF);
    console_fb = framebuffer;
    console_width = fb_terminal_w;
    console_height = fb_terminal_h;
    console_clear();
    console_fullPut();
    printk("Page Fault at 0x%x\n", faulting_address);
    printk("Saved EIP: 0x%x\n", most_recent_int_stack_state.eip);
    printk("[KERNEL PANIC]\n");
    swap_buffers();
    //play_sound(500);
    //for(int i = 0xFFFFFFF; i > 0; i--){}
    //mute();
    asm volatile("hlt");
}

void create_page_entry(
    uint32_t base_address,
    uint32_t target_address,
    uint16_t flag
){
    uint32_t *page_table = (uint32_t*) &boot_page_directory;
    page_table[get_page_index_from_addr(target_address)] = ((base_address & 0xFFC00000) | flag);
    //printk("[Paging] Mapped Addr: %x at Index: %x To Addr: %x Set Value: %x\n", base_address, get_page_index_from_addr(target_address), target_address, ((base_address & 0xFFC00000) | 0x83));
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
        /*
        PAT 0 - Write Back
        PAT 1 - Write Through
        PAT 2 - Write Combining
        PAT 3 - Uncacheable
        PAT 4+- Uncacheable
        */

        /*
        Page Flags

        PAT PCD PWT = ENT
         0   0   0    PAT 0 (Write Back)
         0   0   1    PAT 1 (Write Through)
         0   1   0    PAT 2 (Write Combining)
         0   1   1    PAT 3 (Uncacheable)
         1   0   0    PAT 4
         1   0   1    PAT 5
         1   1   0    PAT 6
         1   1   1    PAT 7

        Flags for 4MB Directory Entry
        Bit Flag
         0  Present (1)
         1  Read/Write (1)
         2  User/Supervisor (0) **ENTIRE OS IS KERNAL MODE**
         3  PWT
         4  PCD
         5  Accessed
         6  Available
         7  Page Size (1)

        Flag Defaults
        PAT 0 - 0x83 WB
        PAT 1 - 0x8B WT
        PAT 2 - 0x93 WC
        PAT 3 - 0x9B UC
        */

        IA32_PAT_low = 0x00010406;
        setMTRR_low(0x277, IA32_PAT_low);
        IA32_PAT_low = getMTRR_low(0x277);
        printk("Set PAT\n");
    }
    else{
        printk("Unable to set PAT\n");
    }
}

void dump_page_map(){
    uint32_t *page_table = (uint32_t *) &boot_page_directory;
    printk("Page Dump:\n");
    printk("IDX |  Virtual -> Physical\n");
    for(uint32_t i = 0; i < 1024; i++){
        if(page_table[i]){
            printk("%3x | %x -> %x | %2x\n", i, i << 22, page_table[i] & 0xFFC00000, page_table[i] & 0x9F);
        }
    }
}
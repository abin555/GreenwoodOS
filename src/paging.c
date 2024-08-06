#include "paging.h"

#include "console.h"
#include "multitasking.h"

uint32_t *page_directory;

extern int8_t task_running_idx;
extern void stop_task(int8_t);

void page_init(){
	print_serial("[Paging] Init\n");

	//uint32_t *page_table = (uint32_t *) &boot_page_directory;
	page_directory = (uint32_t *) &boot_page_directory;
    interrupt_add_handle(14, page_error);

	update_page();

	print_serial("[Paging] Init Finished\n");
}

struct cpu_state page_error(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    print_serial("\nERROR: PAGE FAULT! @ 0x%x\n", stack.eip);
    print_console(kernel_console, "\nERROR: PAGE FAULT! @ 0x%x\n", stack.eip);
    //asm("hlt");
    stop_task(task_running_idx);
    switch_to_task(&tasks[task_running_idx], &tasks[0]);
    print_console(kernel_console, "Returning to kernel Task\n");
    return cpu;
}

uint32_t get_physical(uint32_t address){
    uint32_t lower_addr = address & ~0xFFC00000;
    uint32_t page_dir = address >> 22;
    uint32_t value = page_directory[page_dir];
    value &= 0xFFC00000;
    value |= lower_addr;
    return value;
}

uint32_t get_virtual(uint32_t address){
    //return page_directory[get_page_index_from_addr(address)] & 0xFFC00000;
    for(int i = 0; i < NUM_PAGES; i++){
        if((page_directory[i] & 0xFFC00000) == address){
            return (i << 22) | (address & ~0xFFC00000);
        }
    }
    return 0x68;
}

void create_page_entry(
	uint32_t base_address,
	uint32_t target_address,
	uint16_t flag
){
    //print_serial("[Paging] Base: 0x%x Target 0x%x\n", base_address, target_address);
    page_directory[get_page_index_from_addr(target_address)] = ((base_address & 0xFFC00000) | flag);
    update_page();
}

void delete_page_entry(uint32_t target_address){
    page_directory[get_page_index_from_addr(target_address)] = 0;
    update_page();
}

uint32_t get_page_index_from_addr(uint32_t address){
    return (address >> 22);
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
         2  User/Supervisor (0) **ENTIRE OS IS kernel MODE**
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
        print_serial("Set PAT\n");
    }
    else{
        print_serial("Unable to set PAT\n");
    }
}

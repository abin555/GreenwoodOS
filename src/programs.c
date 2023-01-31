#include "programs.h"
/*
0x02000000 - Slot 1
0x02400000 - Slot 2
0x02800000 - Slot 3
0x02C00000 - Slot 4
0x03000000 - Slot 5
0x03800000 - Slot 6
0x03C00000 - Slot 7
0x04000000 - Slot 8
0x04400000 - Slot 9
0x04800000 - Slot 10
*/
uint8_t program_active_slot = 0;
void exec_user_program(int pid, uint32_t args[10]){
    register uint32_t arg asm("eax")__attribute__((unused)) = args[0];
    asm volatile("call 0x00000000");    
    kill_process(pid);
    console_fullPut();
}

void init_program_memory(){
    printk("[PGRM] Mapping Program Memory\n");
    IRQ_OFF;
    for(uint8_t slot = 0; slot < 10; slot++){
        create_page_entry(0x02000000 + 0x400000*slot, 0x02000000 + 0x400000*slot, 0x83);
    }
    IRQ_RES;
    program_active_slot = 0;
    select_program(0);
}



/*
void load_program(uint8_t program_slot, struct FS_Item_Entry* file){

}
*/
void select_program(uint8_t program_slot){
    create_page_entry(0x02000000 + 0x400000*program_active_slot, 0x02000000 + 0x400000*program_active_slot, 0x83);
    create_page_entry(0x02000000 + 0x400000*program_slot, 0, 0x83);
    program_active_slot = program_slot;
}

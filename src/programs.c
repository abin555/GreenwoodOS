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




void load_program(uint8_t program_slot, struct FS_Item_Entry* file){
            uint8_t *data_buf = (uint8_t*) (0x02000000 + 0x400000*program_slot);

            for(uint32_t sector = 0; sector < file->sector_count; sector++){
                uint8_t* read_buf = ISO_read_sector(file->drive, file->sector+sector);
                for(int index = 0; index < 0x800; index++){
                    data_buf[sector*0x800 + index] = read_buf[index];
                    if(sector * 0x800 + index >= file->size){
                        break;
                    }
                }    
            }
}


bool program_slot_status[10] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
};

int reserveProgramSlot(){
    for(int i = 0; i < 10; i++){
        if(program_slot_status[i] == false){
            program_slot_status[i] = true;
            printk("[PGRM] Reserved Slot %d\n", i);
            return i;
        }
    }
    return -1;
}

void freeProgramSlot(int slot){
    program_slot_status[slot] = false;
    printk("[PGRM] Freed Slot %d\n", slot);
}

void select_program(uint8_t program_slot){
    create_page_entry(0x02000000 + 0x400000*program_active_slot, 0x02000000 + 0x400000*program_active_slot, 0x83);
    create_page_entry(0x02000000 + 0x400000*program_slot, 0, 0x83);
    program_active_slot = program_slot;
}

#include "program.h"

bool program_slot_status[PROGRAM_MAX] = {
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

uint32_t program_region_phys_base;

void program_init(){
	int blockStart = MEM_findRegionIdx(0x400000 * PROGRAM_MAX);
	MEM_reserveRegionBlock(blockStart, 0x400000 * PROGRAM_MAX, PROGRAM_VIRT_REGION_BASE, PROGRAM);
	program_region_phys_base = blockStart * 0x400000;
	print_serial("[PROGRAM] Memory Region Initialized at Physical 0x%x\n", program_region_phys_base);
}

uint8_t program_active_slot = 0;

void select_program(uint8_t program_slot){
    create_page_entry(program_region_phys_base + 0x400000*program_active_slot, PROGRAM_VIRT_REGION_BASE + 0x400000*program_active_slot, 0x83);
    create_page_entry(program_region_phys_base + 0x400000*program_slot, 0, 0x83);
    program_active_slot = program_slot;
}

void exec(char *filename, int argc, char **argv){
	int slot = -1;
	for(int i = 0; i < PROGRAM_MAX; i++){
		if(program_slot_status[i] == false){
			slot = i;
			program_slot_status[i] = true;
			break;
		}
	}
	if(slot == -1) return;
	print_serial("Loading Program %s to slot %d\n", filename, slot);

	struct FILE *file = fopen_rel(&tasks[task_running_idx].currentDirectory, filename);
	if(file == NULL) return;
	fcopy(file, (char *) (PROGRAM_VIRT_REGION_BASE + 0x400000*slot), fsize(file));	
	fclose(file);
	start_task(0, slot, argc, argv, filename);
}

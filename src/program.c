#include "program.h"
#include "elf.h"

bool program_slot_status[PROGRAM_MAX] = {0};

uint32_t program_region_phys_base;
uint32_t program_region_virt_base;

void program_init(){
	int blockStart = MEM_findRegionIdx(0x400000 * PROGRAM_MAX);
	program_region_virt_base = MEM_reserveRegionBlock(blockStart, 0x400000 * PROGRAM_MAX, 0, PROGRAM);
	program_region_phys_base = blockStart * 0x400000;
	print_serial("[PROGRAM] Memory Region Initialized at Physical 0x%x\n", program_region_phys_base);
}

uint8_t program_active_slot = 0;

void __attribute__ ((optimize("-O3"))) select_program(uint8_t program_slot){
	print_serial("[PROGRAM] Selecting %d\n", program_slot);
    create_page_entry(program_region_phys_base + 0x400000*program_active_slot, program_region_virt_base + 0x400000*program_active_slot, 0x83);
    create_page_entry(program_region_phys_base + 0x400000*program_slot, 0, 0x83);
    program_active_slot = program_slot;
}

void exec(char *filename, int argc, char **argv){
	print_serial("\n[EXEC] Starting Program %s\n", filename);
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
	if(elf_check_supported(file)){
		print_serial("[PROGRAM] Is ELF Format!\n");
		fcopy(file, (char *) (program_region_virt_base + 0x400000*slot), fsize(file));
		//memcpy((void *) (PROGRAM_VIRT_REGION_BASE + 0x400000*slot), (void *) (PROGRAM_VIRT_REGION_BASE + 0x400000*slot + 0x1000), fsize(file) - 0x1000);	
		fclose(file);
		start_task((void *)0x1000, slot, argc, argv, filename);
	}
	else {
		fseek(file, 0);
		fcopy(file, (char *) (program_region_virt_base + 0x400000*slot), fsize(file));	
		fclose(file);
		start_task(0, slot, argc, argv, filename);
	}
	print_serial("[EXEC] Program started!\n\n");
}

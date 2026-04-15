#include "program.h"
#include "elf.h"
#include "multitasking.h"
#include "vfs.h"
#include "loader.h"

bool program_slot_status[PROGRAM_MAX] = {0};

uint32_t program_region_phys_base;
uint32_t program_region_virt_base;

void program_init(){
	int blockStart = MEM_findRegionIdx(PROGRAM_MAX_SIZE * PROGRAM_MAX);
	program_region_virt_base = MEM_reserveRegionBlock(blockStart, PROGRAM_MAX_SIZE * PROGRAM_MAX, 0, PROGRAM);
	program_region_phys_base = blockStart * PROGRAM_MAX_SIZE;
	print_serial("[PROGRAM] Memory Region Initialized at Physical 0x%x\n", program_region_phys_base);
	MEM_printRegions();
}

uint8_t program_active_slot = 0;

void __attribute__ ((optimize("-O3"))) select_program(int program_slot){
    create_page_entry(program_region_phys_base + PROGRAM_MAX_SIZE*program_active_slot, program_region_virt_base + PROGRAM_MAX_SIZE*program_active_slot, 0x83);
    create_page_entry(program_region_phys_base + PROGRAM_MAX_SIZE*program_slot, 0, 0x83);
    program_active_slot = program_slot;
}

int program_findSlot(){
	int slot = -1;
	for(int i = 0; i < PROGRAM_MAX; i++){
		if(program_slot_status[i] == false){
			slot = i;
			program_slot_status[i] = true;
			break;
		}
	}
	return slot;
}

int exec(char *filename, int argc, char **argv, void *vctx){
	//struct task_file_ctx *file_ctx = vctx;
	print_serial("\n[EXEC] Starting Program %s\n", filename);	

	//struct FILE *file = fopen_rel(&tasks[task_running_idx].currentDirectory, filename);
	int file = vfs_openRel(&tasks[task_running_idx].currentDirectory, filename, VFS_FLAG_READ);
	vfs_seek(file, 0, 0);
	if(file == -1) return 0;
	int pid = 0;
	pid = loader_loadProgram(file, filename, argc, argv, vctx);
	//print_serial("[EXEC] New Program PID is %d\n", pid);
	return pid;
	//print_serial("[EXEC] Program started!\n\n");
}

int exec_spec(struct exec_spec_ctx *ctx){
	if(ctx == NULL) return -1;
	return exec(
		ctx->filename,
		ctx->argc,
		ctx->argv,
		ctx->file_ctx
	);
}
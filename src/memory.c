#include "memory.h"


struct memory_region MEMORY_REGIONS[MEMORY_NUM_REGIONS];

void memset(void *address, uint8_t value, uint32_t size){
	uint8_t *mem = (uint8_t *) address;
	for(uint32_t i = 0; i < size; i++){
		mem[i] = value;
	}
}

void memcpy(void *dest, void* src, int size){
	for(int i = 0; i < size; i++){
		((uint8_t *) dest)[i] = ((uint8_t *)src)[i];
	}
}

void memfcpy(void* target, void* source, uint32_t size){
    asm volatile ("rep movsb"
    : "=D" (target),
    "=S" (source),
    "=c" (size)
    : "0" (target),
    "1" (source),
    "2" (size)
    : "memory");
}

int MEM_reserveRegion(uint32_t physical, uint32_t virtual, MEMORY_REGION_TYPE type){
	uint32_t idx = physical >> 22;
	uint32_t flag;
	switch(type){
		case FRAMEBUFFER:
			flag = 0x93;
		break;
		default:
			flag = 0x83;
		break;
	}
	//if(!MEMORY_REGIONS[idx].available && !MEMORY_REGIONS[idx].exists && type != FRAMEBUFFER) return -1;
	MEMORY_REGIONS[idx].exists = 1;
	MEMORY_REGIONS[idx].available = 0;
	MEMORY_REGIONS[idx].type = type;
	MEMORY_REGIONS[idx].physical_addr = physical;
	MEMORY_REGIONS[idx].virtual_addr = virtual;

	print_serial("[MEM] Reserved Region of Type %x at physical 0x%x and virtual 0x%x Flag: %x Region IDX: %x\n", type, physical, virtual, flag, idx);
	create_page_entry(physical, virtual, flag);
	return 0;
}

void MEM_populateRegions(){
	print_serial("Loading Memory Map into Memory Region Table\n");
	for(int i = 0; i < 10; i++){
        MEMORY_REGIONS[i].exists = 1;
		MEMORY_REGIONS[i].available = 0;
        MEMORY_REGIONS[i].type = KERNEL;
        MEMORY_REGIONS[i].physical_addr = i*0x400000;
        MEMORY_REGIONS[i].virtual_addr = get_virtual(MEMORY_REGIONS[i].physical_addr);
    }
	for(int i = 15; i < 0x300; i++){
		MEMORY_REGIONS[i].exists = 1;
		MEMORY_REGIONS[i].available = 1;
        MEMORY_REGIONS[i].type = AVAILABLE;
        MEMORY_REGIONS[i].physical_addr = i*0x400000;
        MEMORY_REGIONS[i].virtual_addr = get_virtual(MEMORY_REGIONS[i].physical_addr);
	}
	return;
	int num_blocks = (GRUB_MMAP->size) / GRUB_MMAP->entry_size;
	print_serial("Total Size: 0x%x\nEntry Size: 0x%x\nNum Blocks: 0x%x\n", GRUB_MMAP->size, GRUB_MMAP->entry_size, num_blocks);
	for(int i = 0; i < num_blocks; i++){
		print_serial(
			"[MEM REGION] Base Address: 0x%x\n[MEM REGION] Length: 0x%x\n[MEM REGION] Type: 0x%x\n", 
			GRUB_MMAP->entries[i].upper_addr, 
			GRUB_MMAP->entries[i].upper_len, 
			GRUB_MMAP->entries[i].type);
		if(GRUB_MMAP->entries[i].type == MULTIBOOT_MEMORY_BADRAM){
			print_serial("[MEM REGION] is unavailable\n");
			continue;
		}
		int table_idx = GRUB_MMAP->entries[i].upper_addr >> 22;
		int contains_count = (GRUB_MMAP->entries[i].upper_len / PAGE_SIZE) + 1;
		print_serial("Table IDX %x with %x regions\n", table_idx, contains_count);
		for(int j = 0; j < contains_count; j++){
			MEMORY_REGION_TYPE type;
			switch(GRUB_MMAP->entries[i].type){
				case MULTIBOOT_MEMORY_AVAILABLE:
					type = AVAILABLE;
					break;
				case MULTIBOOT_MEMORY_RESERVED:
					type = KERNEL;
					break;
				default:
					type = OTHER;
					break;
			}
			MEMORY_REGIONS[table_idx + j].exists = 1;
			MEMORY_REGIONS[table_idx + j].type = type;
			MEMORY_REGIONS[table_idx + j].physical_addr = (GRUB_MMAP->entries[i].upper_addr + PAGE_SIZE * j) & 0xFFC00000;
			MEMORY_REGIONS[table_idx + j].virtual_addr = get_virtual(MEMORY_REGIONS[table_idx + j].physical_addr);
			if(type == AVAILABLE){
				MEMORY_REGIONS[table_idx + j].available = 1;
			}
			else{
				MEMORY_REGIONS[table_idx + j].available = 0;
			}
			//print_serial("IDX %x is type %x at PHYS 0x%x VIRT: 0x%x\n", table_idx + j, type, MEMORY_REGIONS[table_idx + j].physical_addr, MEMORY_REGIONS[table_idx + j].virtual_addr);
		}
	}

	print_serial("Loading Existing Page Table into Memory Region Table\n");
	for(int i = 0; i < NUM_PAGES; i++){
		if(page_directory[i] == 0){

		}
		else{
			uint32_t physical_memory = page_directory[i] & 0xFFC00000;
			uint32_t virtual_memory = i << 22;
			uint32_t mem_idx = physical_memory >> 22;
			print_serial("Page IDX: %x MEM IDX: %x PHYS: 0x%x VIRT: 0x%x\n", i, mem_idx, physical_memory, virtual_memory);
			MEMORY_REGIONS[mem_idx].exists = 1;
			MEMORY_REGIONS[mem_idx].available = 0;
			MEMORY_REGIONS[mem_idx].type  = KERNEL;
			MEMORY_REGIONS[mem_idx].physical_addr = physical_memory;
			MEMORY_REGIONS[mem_idx].virtual_addr = virtual_memory;
		}
		
		//MEMORY_REGIONS[i] = region;
	}
}
#include "console.h"
void MEM_printRegions(){
	for(int i = 0; i < MEMORY_NUM_REGIONS; i++){
		if(MEMORY_REGIONS[i].exists && !MEMORY_REGIONS[i].available){
			char *type;
			switch(MEMORY_REGIONS[i].type){
				case KERNEL:
					type = "  KERNEL   ";
					break;
				case DRIVER:
					type = "  DRIVER   ";
					break;
				case ALLOC:
					type = "  ALLOC    ";
					break;
				case SYSTEM:
					type = "  SYSTEM   ";
					break;
				case OTHER:
					type = "  OTHER    ";
					break;
				case PROGRAM:
					type = "  PROGRAM  ";
					break;
				case UNDEFINED:
					type = " UNDEFINED ";
					break;
				case FRAMEBUFFER:
					type = "FRAMEBUFFER";
					break;
				case AVAILABLE:
					type = " AVAILABLE ";
					break;
			}
			print_serial("[MEM] Region %x is type [ %s ] at PHYS: 0x%x VIRT: 0x%x\n", i, type, MEMORY_REGIONS[i].physical_addr, MEMORY_REGIONS[i].virtual_addr);
			print_console(kernel_console, "[MEM] Region %d is type [%s] at PHYS: 0x%x VIRT: 0x%x\n", i, type, MEMORY_REGIONS[i].physical_addr, MEMORY_REGIONS[i].virtual_addr);
		}
	}
}

int calculateBlocks(uint32_t size){
	int small_alloc = 0;
	if(size % PAGE_SIZE) small_alloc = 1;
	return size / PAGE_SIZE + small_alloc;
}

int MEM_findRegionIdx(uint32_t size){
	int needed_blocks = calculateBlocks(size);
	print_serial("Looking for Memory region of size %x = %x blocks\n", size, needed_blocks);
	//printk("Looking for Memory region of size %x = %x blocks\n", size, needed_blocks);
	int check_idx = -1;
	int check_count = -1;
	for(int i = 0; i < MEMORY_NUM_REGIONS; i++){
		if(!MEMORY_REGIONS[i].available || !MEMORY_REGIONS[i].exists){
			check_idx = -1;
			check_count = -1;
			continue;
		}
		if(check_idx == -1){
			check_idx = i;
			check_count = 0;
		}
		check_count++;
		if(check_count == needed_blocks){
			print_serial("Found region of size %x at idx %x to %x\n");
			return check_idx;
		}
	}
	return -1;
}

uint32_t MEM_reserveRegionBlock(int idx, uint32_t size, uint32_t virtual_base, MEMORY_REGION_TYPE type){
	uint32_t start_physical = idx * PAGE_SIZE;
	int needed_blocks = calculateBlocks(size);
	if(virtual_base == 0) virtual_base = start_physical;
	for(int i = 0; i < needed_blocks; i++){
		MEM_reserveRegion(start_physical + i * PAGE_SIZE, virtual_base + i * PAGE_SIZE, type);
	}
	return start_physical;
}
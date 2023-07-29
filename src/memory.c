#include "memory.h"

void MEM_populateRegion(){
	print_serial("Loading Memory Map into Memory Region Table\n");
	int num_blocks = (GRUB_MMAP->size) / GRUB_MMAP->entry_size;
	print_serial("Total Size: 0x%x\nEntry Size: 0x%x\nNum Blocks: 0x%x\n", GRUB_MMAP->size, GRUB_MMAP->entry_size, num_blocks);
	for(int i = 0; i < num_blocks; i++){
		print_serial(
			"[MEM REGION] Base Address: 0x%x\n[MEM REGION] Length: 0x%x\n[MEM REGION] Type: 0x%x\n", 
			GRUB_MMAP->entries[i].upper_addr, 
			GRUB_MMAP->entries[i].upper_len, 
			GRUB_MMAP->entries[i].type);
	}
}
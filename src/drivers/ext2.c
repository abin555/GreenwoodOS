#include "ext2.h"

void *ext2_read_block(struct EXT2_FS *ext2, uint32_t block_id){
	//print_serial("[EXT2] Reading block %d\n", block_id);
	if(block_id > ext2->block_count) return NULL;
	memset(ext2->buf, 0, ext2->block_size);
	int err = drive_read(ext2->drive, ext2->buf, block_id*ext2->sectors_per_block, ext2->sectors_per_block);
	if(err == -1) return NULL;
	return ext2->buf;
}

int ext2_write_block(struct EXT2_FS *ext2, uint32_t block_id, void *buf){
	//print_serial("[EXT2] Writing block %d\n", block_id);
	if(block_id > ext2->block_count) return 0;
	int err = drive_write(ext2->drive, buf, block_id*ext2->sectors_per_block, ext2->sectors_per_block);
	if(err == -1) return 0;
	return 1;
}

struct EXT2_Inode ext2_read_inode_data(struct EXT2_FS *ext2, uint32_t inodeIdx){
	uint32_t group = (inodeIdx - 1) / ext2->inodes_per_group;
	uint32_t inode_table_block = ext2->inode_table_starting_addr[group];
	uint32_t idx_in_group = inodeIdx - group * ext2->inodes_per_group;
	uint32_t block_offset = (idx_in_group - 1) * ext2->inode_size / ext2->block_size;
	uint32_t offset_in_block = (idx_in_group - 1) - block_offset * (ext2->block_size / ext2->inode_size);
	char *block = ext2_read_block(ext2, inode_table_block + block_offset);
	struct EXT2_Inode *inode = ((struct EXT2_Inode *) (block + offset_in_block * ext2->inode_size));
	//print_serial("[EXT2] Inode %d: Group %d, Type: 0x%x, Size: %d, Block: %d\n", inodeIdx, group, inode->type_perms & 0xF000, inode->lsbSize, inode_table_block + block_offset);
	return *inode;
}

void ext2_write_inode_data(struct EXT2_FS *ext2, struct EXT2_Inode *inode, uint32_t inodeIdx){
	if(ext2 == NULL || inode == NULL) return;
	//print_serial("[EXT2] Writing Inode %d from 0x%x\n", inodeIdx, inode);
	uint32_t group = (inodeIdx - 1) / ext2->inodes_per_group;
	uint32_t inode_table_block = ext2->inode_table_starting_addr[group];
	uint32_t idx_in_group = inodeIdx - group * ext2->inodes_per_group;
	uint32_t block_offset = (idx_in_group - 1) * ext2->inode_size / ext2->block_size;
	uint32_t offset_in_block = (idx_in_group - 1) - block_offset * (ext2->block_size / ext2->inode_size);
	char *block = ext2_read_block(ext2, inode_table_block + block_offset);
	struct EXT2_Inode *inode_replace = ((struct EXT2_Inode *) (block + offset_in_block * ext2->inode_size));
	//print_serial("[EXT2] Inode Write - Size: %d / %d\n", inode_replace->lsbSize, inode->lsbSize);
	memcpy(inode_replace, inode, sizeof(struct EXT2_Inode));
	ext2_write_block(ext2, inode_table_block + block_offset, block);
}

void ext2_print_directory(struct EXT2_FS *ext2, uint32_t inodeIdx){
	print_serial("[EXT2] Printing Directory from inode %d\n", inodeIdx);
	struct EXT2_Inode inode = ext2_read_inode_data(ext2, inodeIdx);
	if((inode.type_perms & 0xF000) != EXT2_InodeType_Directory) return;
	
	void *directory_entries = ext2_read_block(ext2, inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;
	while(dir->entry_size != 0){
		print_serial("[EXT2] Inode %d Type %d - ", dir->inode, dir->type);

		for(uint8_t i = 0; i < dir->name_length; i++){
			print_serial("%c", dir->name[i]);
		}
		print_serial("\n");

		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
}

uint32_t ext2_get_direntry_inode(struct EXT2_FS *ext2, uint32_t parent_inodeIdx, char *name){
	//print_serial("[EXT2] Looking for %s from inode %d\n", name, parent_inodeIdx);
	//print_console(kernel_console, "[EXT2] Looking for %s from inode %d\n", name, parent_inodeIdx);
	struct EXT2_Inode parent_inode = ext2_read_inode_data(ext2, parent_inodeIdx);
	void *directory_entries = ext2_read_block(ext2, parent_inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;

	char work_buf[20];

	while(dir->entry_size != 0){
		memset(work_buf, 0, sizeof(work_buf));
		memcpy(work_buf, dir->name, dir->name_length);
		//print_serial("[EXT2] Inode %d Type %d - %s\n", dir->inode, dir->type, work_buf);
		//print_console(kernel_console, "[EXT2] Inode %d Type %d - %s\n", dir->inode, dir->type, work_buf);

		if(strcmp(name, work_buf) == 0){
			return dir->inode;
		}

		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
	return 0;
}

void ext2_debug_print_inode(struct EXT2_Inode *inode){
	print_serial("[EXT2] Type Perms %x\n", inode->type_perms);
	print_serial("[EXT2] User Id %x\n", inode->userId);
	print_serial("[EXT2] Lsb Size %x\n", inode->lsbSize);
	print_serial("[EXT2] Last Access %x\n", inode->lastAccessTime);
	print_serial("[EXT2] Creation %x\n", inode->creationTime);
	print_serial("[EXT2] Modification %x\n", inode->lastModificationTime);
	print_serial("[EXT2] Deletion %x\n", inode->deletionTime);
	print_serial("[EXT2] GroupID %x\n", inode->GroupID);
	print_serial("[EXT2] Hardlink # %x\n", inode->hardLinkCount);
	print_serial("[EXT2] Disk Sectors %x\n", inode->diskSectorsCount);
	print_serial("[EXT2] flags %x\n", inode->flags);
	print_serial("[EXT2] OSval %x\n", inode->OSval);

	for(int i = 0; i < 15; i++)
		print_serial("[EXT2] BP%d %x\n", i, inode->BlockPointers[i]);

	print_serial("[EXT2] Gen Num %x\n", inode->GenerationNumber);
	print_serial("[EXT2] Attributes %x\n", inode->ExtendedAttributes);
	print_serial("[EXT2] Msb Size %x\n", inode->msbSize);
	print_serial("[EXT2] Fragment Block %x\n", inode->fragmentBlock);

}

void ext2_console_printDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, uint32_t inodeIdx){
	struct EXT2_Inode inode = ext2_read_inode_data(ext2, inodeIdx);
	if((inode.type_perms & 0xF000) != EXT2_InodeType_Directory){
		print_console(console, "File of size %d\n", inode.lsbSize);
		return;
	};
	if(*((int *) &inode.lsbSize) < 0){
		//print_serial("[EXT2] Error reading directory!\n");
		print_console(console, "[EXT2] Error reading directory!\n");
		return;
	}
	
	void *directory_entries = ext2_read_block(ext2, inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;

	char work_buf[21];
	//memset(work_buf, 0, sizeof(work_buf));

	ext2_debug_print_inode(&inode);

	int i = 0;
	while(dir->entry_size != 0 && i < 50){
		i++;
		memset(work_buf, 0, sizeof(work_buf));
		memcpy(work_buf, dir->name, dir->name_length <= 20 ? dir->name_length : 20);
		//for(uint8_t i = 0; i < dir->name_length; i++){
		//	print_serial("%c", dir->name[i]);
		//}
		//print_serial("\n");
		print_serial("%s %d\n", work_buf, dir->entry_size);
		print_console(console, "%s\n", work_buf);
		//print_serial("[EXT2] [LS] Inode %d %s %x\n", inodeIdx, work_buf, (uint32_t) dir);
		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
}

int ext2_check_format(struct DRIVE *drive){
	print_serial("[EXT2] Checking format on drive %c\n", drive->identity);
	if(drive == NULL){
		return 0;
	}
	int err = drive_read(drive, drive_sector_buf, 2, 1);
	if(!err) return 0;
	struct ext2_super_block *super_block = (struct ext2_super_block *) drive_sector_buf;
	if(!(super_block->magic == EXT2_SUPER_MAGIC)) return 0;
	print_serial("[EXT2] Drive %c matches magic number!\n", drive->identity);

	struct EXT2_FS *ext2 = (struct EXT2_FS *) malloc(sizeof(struct EXT2_FS));
	drive->format = EXT2;
	drive->format_info.ext2 = ext2;
	ext2->drive = drive;
	
	ext2->block_size = 1024 << super_block->log_block_size;
	ext2->sectors_per_block = ext2->block_size / 0x200;
	//ext2->buf = (char *) malloc(sizeof(char) * ext2->block_size);
	ext2->buf = (char *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(sizeof(char) * ext2->block_size), ext2->block_size, 0, SYSTEM));
	memset(ext2->buf, 0, PAGE_SIZE/2);
	ext2->block_count = super_block->blocks_count;
	ext2->inode_count = super_block->inodes_count;
	ext2->starting_block_number = super_block->first_data_block;
	ext2->blocks_per_group = super_block->blocks_per_group;
	
	ext2->inodes_per_group = super_block->inodes_per_group;
	ext2->ext2_version = super_block->rev_level;
	ext2->group_count = 1 + (super_block->blocks_count-1) / super_block->blocks_per_group;
	ext2->first_inode = super_block->first_ino;

	if(ext2->ext2_version < 1){
		ext2->inode_size = 128;
	}
	else{
		ext2->inode_size = super_block->inode_size;
	}

	ext2->inode_table_starting_addr = malloc(ext2->group_count * sizeof(uint32_t));

	print_serial("[EXT2] Block Size: %d, Block Count: %d, Inode Count: %d, Starting Block Number: %d, Blocks Per Group: %d, First Inode: %d, Inodes Per Group: %d\n", 
	ext2->block_size, ext2->block_count, ext2->inode_count, ext2->starting_block_number, ext2->blocks_per_group, ext2->first_inode, ext2->inodes_per_group);
	print_serial("[EXT2] Inodes Per Group: %d\n", ext2->inodes_per_group);

	void *BGD_block = ext2_read_block(ext2, ext2->starting_block_number+1);
	struct EXT2_BlockGroupDescriptor *BGD = BGD_block;
	ext2->block_addr_block_usage_map = malloc(sizeof(uint32_t) * ext2->group_count);
	ext2->block_addr_inode_usage_map = malloc(sizeof(uint32_t) * ext2->group_count);
	ext2->inode_table_starting_addr = malloc(sizeof(uint32_t) * ext2->group_count);
	print_serial("[EXT2] BGD size is %d\n", sizeof(struct EXT2_BlockGroupDescriptor));
	for(uint32_t i = 0; i < ext2->group_count; i++){
		if((uint32_t) BGD - (uint32_t) BGD_block >= ext2->block_size){
			BGD_block = ext2_read_block(ext2, ext2->starting_block_number+1+(i*sizeof(struct EXT2_BlockGroupDescriptor) / ext2->block_size));
			BGD = BGD_block;
		}
		ext2->block_addr_block_usage_map[i] = BGD->block_usage_addr;
		ext2->block_addr_inode_usage_map[i] = BGD->inode_usage_addr;
		ext2->inode_table_starting_addr[i] = BGD->inode_table_addr;
		BGD = BGD + 1;
	}
	print_serial("[EXT2] Inode map at %d Block map at %d\n", ext2->block_addr_inode_usage_map[0], ext2->block_addr_block_usage_map[0]);
	print_serial("[EXT2] Free inode test %d\n", ext2_get_freeInodeIdx(ext2));
	print_serial("[EXT2] Free block test %d\n", ext2_get_freeBlockIdx(ext2));
	return 1;
}


uint32_t ext2_get_inodeIdx_from_path(struct EXT2_FS *ext2, char *path){
	uint32_t working_idx = EXT2_ROOT_INODE;
	static char work_buf[50];
	int path_component_size;
	int iterations = 0;
	while(*path != '\0'){
		memset(work_buf, 0, sizeof(work_buf));
		path_component_size = 0;
		//memcpy(work_buf, dir->name, dir->name_length);
		for(int i = 0; path[i] != '\0' && path[i] != '/' && i < 50; i++){
			work_buf[i] = path[i];
			path_component_size++;
		}
		working_idx = ext2_get_direntry_inode(ext2, working_idx, work_buf);
		if(working_idx == 0){
			print_console(kernel_console, "[EXT2] Next is Zero? %s\n", work_buf);
			return 0;
		}
		//print_serial("[EXT2] Solving Path Iter %d Partial Path: %s Working Inode Idx: %d\n", iterations, work_buf, working_idx);
		//print_console(kernel_console, "[EXT2] Solving Path Iter %d Partial Path: %s Working Inode Idx: %d\n", iterations, work_buf, working_idx);
		path += path_component_size + 1;
		iterations++;
	}
	return working_idx;
}


void ext2_listDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, char *path){
	//print_console(console, "Listing Drive %c path %s\n", ext2->drive->identity, path);
	uint32_t inodeIdx = ext2_get_inodeIdx_from_path(ext2, path);
	//print_serial("[EXT2] entry is at inode %d\n", inodeIdx);
	if(inodeIdx == 0){
		print_console(console, "%s does not exist\n", path);
		return;
	}
	ext2_console_printDirectory(console, ext2, inodeIdx);
}


uint32_t ext2_get_freeInodeIdx(struct EXT2_FS *ext2){
	uint32_t byte_block_idx = 0;
	uint8_t byte_block = 0;
	uint8_t inode_bit_idx = 0;
	uint8_t inode_alloc_state = 0;

	uint32_t group = 4;
	uint32_t temp_group = 4;
	uint8_t *inode_map = ext2_read_block(ext2, ext2->block_addr_inode_usage_map[group]);

	for(uint32_t i = 11; i < ext2->inode_count; i++){
		temp_group = (i - 1) / ext2->inodes_per_group;
		if(group != temp_group){
			group = temp_group;
			inode_map = ext2_read_block(ext2, ext2->block_addr_inode_usage_map[group]);
		}
		byte_block_idx = (i - 1) / 8;
		inode_bit_idx = 1 << (i - 1) % 8;
		byte_block = inode_map[byte_block_idx];
		inode_alloc_state = (byte_block & inode_bit_idx) ? 1 : 0;
		if(inode_alloc_state == 0){
			//print_serial("[EXT2] Found free inode at %d\n", i);
			return i;
		}
		//print_serial("%d %d %x\n", i, inode_alloc_state, byte_block);
	}
	return 0;
}

void ext2_make_dir_entry(struct EXT2_FS *ext2, uint32_t dirInodeIdx, char *name, uint32_t childInodeIdx, uint8_t type, bool new_dir){
	//print_serial("[EXT2] Make directory %s\n", name);
	struct EXT2_Inode dir_inode = ext2_read_inode_data(ext2, dirInodeIdx);
	if((dir_inode.type_perms & 0xF000) != EXT2_InodeType_Directory) return;
	void *block = ext2_read_block(ext2, dir_inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) block;
	char text_buf[20];
	if(!new_dir && dir->entry_size != 0){
		while(ext2->block_size - dir->entry_size - ((uint32_t) dir - (uint32_t) block) != 0){
			memset(text_buf, 0, sizeof(text_buf));
			memcpy(text_buf, dir->name, dir->name_length);
			//print_serial("[EXT2] MKDIR Skipping %s entry size: %d (%x)\n", text_buf, dir->entry_size, ext2->block_size - dir->entry_size - ((uint32_t) dir - (uint32_t) block));
			dir = (struct EXT2_Directory *) ((uint32_t) dir + dir->entry_size);
		}
		dir->entry_size = sizeof(struct EXT2_Directory) + dir->name_length;
		dir->entry_size += 4 - dir->entry_size % 4;
		dir = (struct EXT2_Directory *) ((uint32_t) dir + dir->entry_size); 
	}	

	dir->inode = childInodeIdx;
	dir->name_length = 0;
	dir->type = type;
	dir->entry_size = ext2->block_size - ((uint32_t) dir - (uint32_t) block);
	char *dir_name = name;
	while(*dir_name != '\0'){
		dir->name[dir->name_length] = *dir_name;
		dir_name++;
		dir->name_length++;
	}
	ext2_write_block(ext2, dir_inode.BlockPointers[0], block);
	memset(block, 0, ext2->block_size);
	print_serial("[EXT2] Made directory %s from inode parent %d of inode %d\n", name, dirInodeIdx, childInodeIdx);
}

uint32_t ext2_alloc_inode(struct EXT2_FS *ext2, uint16_t type, uint32_t size){
	//print_serial("[EXT2] Inode Alloc Request\n");
	uint32_t inodeIdx = ext2_get_freeInodeIdx(ext2);
	uint32_t group = (inodeIdx - 1) / ext2->inodes_per_group;

	uint8_t *inode_map = ext2_read_block(ext2, ext2->block_addr_inode_usage_map[group]);
	uint32_t byte_block_idx = (inodeIdx - 1) / 8;
	uint8_t inode_bit_idx = 1 << (inodeIdx - 1) % 8;
	inode_map[byte_block_idx] |= inode_bit_idx;
	if(!ext2_write_block(ext2, ext2->block_addr_inode_usage_map[group], inode_map)) return 0;

	uint32_t inode_table_block = ext2->inode_table_starting_addr[group];
	uint32_t idx_in_group = inodeIdx - group * ext2->inodes_per_group;
	uint32_t block_offset = (idx_in_group - 1) * ext2->inode_size / ext2->block_size;
	uint32_t offset_in_block = (idx_in_group - 1) - block_offset * (ext2->block_size / ext2->inode_size);
	char *block = ext2_read_block(ext2, inode_table_block + block_offset);
	struct EXT2_Inode *inode = ((struct EXT2_Inode *) (block + offset_in_block * ext2->inode_size));
	
	inode->type_perms = type;
	inode->lsbSize = size;
	inode->lastAccessTime = 100;
	inode->creationTime = 100;
	inode->lastModificationTime = 100;
	inode->deletionTime = 0xFFFFFFFF;
	inode->diskSectorsCount = (size / ext2->block_size + 1) * ext2->sectors_per_block;

	for(int i = 0; i < 15; i++){
		inode->BlockPointers[i] = 0;
	}
	inode->msbSize = 0;

	if(!ext2_write_block(ext2, inode_table_block + block_offset, block)) return 0;

	uint32_t BlockPointers[15];
	memset(BlockPointers, 0, sizeof(BlockPointers));

	if(type == EXT2_InodeType_Directory){
		BlockPointers[0] = ext2_alloc_block(ext2);
	}
	else if(type == EXT2_InodeType_RegularFile){
		print_serial("[EXT2] Creating Regular File of size %d\n", size);
		for(uint32_t i = 0; i < size / ext2->block_size + 1 && i < 12; i++){
			BlockPointers[i] = ext2_alloc_block(ext2);
		}
	}
	block = ext2_read_block(ext2, inode_table_block + block_offset);
	inode = ((struct EXT2_Inode *) (block + offset_in_block * ext2->inode_size));
	for(int i = 0; i < 15; i++){
		inode->BlockPointers[i] = BlockPointers[i];
	}
	if(!ext2_write_block(ext2, inode_table_block + block_offset, block)) return 0;
	return inodeIdx;
}


uint32_t ext2_get_freeBlockIdx(struct EXT2_FS *ext2){
	uint32_t byte_block_idx = 0;
	uint8_t byte_block = 0;
	uint8_t block_bit_idx = 0;
	uint8_t block_alloc_state = 0;

	uint32_t group = 4;
	uint32_t temp_group = 4;
	//print_serial("[EXT2 BLOCK ALLOC] Getting Block Map\n");
	uint8_t *block_map = NULL;// = ext2_read_block(ext2, ext2->block_addr_block_usage_map[group]);

	for(uint32_t i = 0; i < ext2->block_count; i++){
		temp_group = i / ext2->blocks_per_group;
		if(group != temp_group){
			group = temp_group;
			block_map = ext2_read_block(ext2, ext2->block_addr_block_usage_map[group]);
		}
		if(block_map == NULL) return 1;
		byte_block_idx = i / 8;
		block_bit_idx = 1 << i % 8;
		byte_block = block_map[byte_block_idx];
		block_alloc_state = (byte_block & block_bit_idx) ? 1 : 0;
		if(block_alloc_state == 0){
			//print_serial("[EXT2] Free Block at %d\n", i);
			return i;
		}
	}
	return 0;
}


uint32_t ext2_alloc_block(struct EXT2_FS *ext2){
	uint32_t blockIdx = ext2_get_freeBlockIdx(ext2);
	uint32_t group = blockIdx / ext2->blocks_per_group;

	uint8_t *block_map = ext2_read_block(ext2, ext2->block_addr_block_usage_map[group]);
	uint32_t byte_block_idx = blockIdx / 8;
	uint8_t block_bit_idx = 1 << blockIdx % 8;
	block_map[byte_block_idx] |= block_bit_idx;
	if(!ext2_write_block(ext2, ext2->block_addr_block_usage_map[group], block_map)) return 0;
	void *block = ext2_read_block(ext2, blockIdx);
	memset(block, 0, ext2->block_size);
	ext2_write_block(ext2, blockIdx, block);
	//print_serial("[EXT2] Free block alloced at %d\n", blockIdx);
	return blockIdx;
}

int ext2_createDirectory(struct EXT2_FS *ext2, char *path){
	print_serial("[EXT2] Creating Directory %s\n", path);
	if(ext2_get_inodeIdx_from_path(ext2, path)){
		print_serial("[EXT2] %s already exists\n", path);
		return 1;
	}
	uint32_t path_length = 0;
	for(int i = 0; path[i] != '\0' && i < 255; i++){
		path_length++;
	}

	int dir_name_idx = 0;
	for(uint32_t i = 0; i < path_length; i++){
		if(path[i] == '/' && path[i+1] != '\0') dir_name_idx = i+1;
	}

	char parent_path[50];
	memset(parent_path, '\0', sizeof(parent_path));
	parent_path[0] = '.';
	parent_path[1] = '/';
	for(int i = 0; i < dir_name_idx && i < (int) sizeof(parent_path); i++){
		parent_path[i] = path[i];
	}

	//print_serial("[EXT2] Making Directory named %s from parent %s\n", path+dir_name_idx, parent_path);
	uint32_t parent_inode_idx = ext2_get_inodeIdx_from_path(ext2, parent_path);
	if(parent_inode_idx == 0) return 1;
	uint32_t child_inode_idx = ext2_alloc_inode(ext2, EXT2_InodeType_Directory, 0);
	if(child_inode_idx == 0) return 1;
	//print_serial("[EXT2] Parent Inode %d, Child Inode is now %d\n", parent_inode_idx, child_inode_idx);
	//print_serial("[EXT2] Creating New Directory\n");
	ext2_make_dir_entry(ext2, parent_inode_idx,path+dir_name_idx, child_inode_idx, 2, false);
	//print_serial("[EXT2] Creating Self Reference Directory\n");
	ext2_make_dir_entry(ext2, child_inode_idx, ".", child_inode_idx, 2, true);
	//print_serial("[EXT2] Creating Reference to Parent Directory\n");
	ext2_make_dir_entry(ext2, child_inode_idx, "..", parent_inode_idx, 2, false);
	return 0;
}

int ext2_createFile(void *fs, char *path, uint32_t size){
	struct EXT2_FS *ext2 = fs;
	print_serial("[EXT2] Creating File %s\n", path);
	if(ext2_get_inodeIdx_from_path(ext2, path)){
		print_serial("[EXT2] %s already exists\n", path);
		return -1;
	}
	uint32_t path_length = 0;
	for(int i = 0; path[i] != '\0' && i < 255; i++){
		path_length++;
	}

	int dir_name_idx = 0;
	for(uint32_t i = 0; i < path_length; i++){
		if(path[i] == '/' && path[i+1] != '\0') dir_name_idx = i+1;
	}

	char parent_path[50];
	memset(parent_path, '\0', sizeof(parent_path));
	parent_path[0] = '.';
	parent_path[1] = '/';
	for(int i = 0; i < dir_name_idx && i < (int) sizeof(parent_path); i++){
		parent_path[i] = path[i];
	}

	//print_serial("[EXT2] Making File named %s from parent %s\n", path+dir_name_idx, parent_path);
	uint32_t parent_inode_idx = ext2_get_inodeIdx_from_path(ext2, parent_path);
	if(parent_inode_idx == 0) return -1;
	uint32_t child_inode_idx = ext2_alloc_inode(ext2, EXT2_InodeType_RegularFile, size);
	if(child_inode_idx == 0) return -1;
	//print_serial("[EXT2] Parent Inode %d, Child Inode is now %d\n", parent_inode_idx, child_inode_idx);
	ext2_make_dir_entry(ext2, parent_inode_idx, path+dir_name_idx, child_inode_idx, 1, false);
	return 0;
}

int ext2_extendFile(struct EXT2_FS *ext2, uint32_t inodeIdx, uint32_t extendAmount){
	struct EXT2_Inode inode = ext2_read_inode_data(ext2, inodeIdx);
	uint32_t current_size = inode.lsbSize;
	uint32_t new_size = current_size + extendAmount;
	uint32_t current_block_count = (current_size / ext2->block_size + 1);
	uint32_t new_block_count = (new_size / ext2->block_size + 1);
	uint32_t blocks_needed = new_block_count - current_block_count;

	print_serial("[EXT2] Extending File from inode %d by size %d currently has %d blocks, adding %d blocks\n", inodeIdx, extendAmount, current_block_count, blocks_needed);

	inode.lsbSize += extendAmount;
	inode.diskSectorsCount += blocks_needed * ext2->block_size / 512;
	if(current_block_count >= 12) return 1;//TODO: Single and Doublely indirect links to blocks

	uint32_t block_idx = current_block_count + 1;
	for(uint32_t i = 0; i < blocks_needed; i++){
		if(block_idx + i >= 12) break;
		//print_serial("[EXT2] Block Idx %d\n", block_idx + i);
		inode.BlockPointers[block_idx + i] = ext2_alloc_block(ext2);
		
	}
	ext2_write_inode_data(ext2, &inode, inodeIdx);
	return 0;
}

/*
void ext2_console_printDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, uint32_t inodeIdx){
	struct EXT2_Inode inode = ext2_read_inode_data(ext2, inodeIdx);
	if((inode.type_perms & 0xF000) != EXT2_InodeType_Directory){
		print_console(console, "File of size %d\n", inode.lsbSize);
		return;
	};
	if(*((int *) &inode.lsbSize) < 0){
		//print_serial("[EXT2] Error reading directory!\n");
		print_console(console, "[EXT2] Error reading directory!\n");
		return;
	}
	
	void *directory_entries = ext2_read_block(ext2, inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;

	char work_buf[21];
	//memset(work_buf, 0, sizeof(work_buf));

	ext2_debug_print_inode(&inode);

	int i = 0;
	while(dir->entry_size != 0 && i < 20){
		i++;
		memset(work_buf, 0, sizeof(work_buf));
		memcpy(work_buf, dir->name, dir->name_length <= 20 ? dir->name_length : 20);
		//for(uint8_t i = 0; i < dir->name_length; i++){
		//	print_serial("%c", dir->name[i]);
		//}
		//print_serial("\n");
		print_serial("%s %d\n", work_buf, dir->entry_size);
		print_console(console, "%s\n", work_buf);
		//print_serial("[EXT2] [LS] Inode %d %s %x\n", inodeIdx, work_buf, (uint32_t) dir);
		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
}
*/

struct DirectoryListing ext2_advListDirectory(void *fs, char *path){
	struct EXT2_FS *ext2 = fs;
	struct DirectoryListing listing = {0};
	//print_console(console, "Listing Drive %c path %s\n", ext2->drive->identity, path);
	uint32_t inodeIdx = ext2_get_inodeIdx_from_path(ext2, path);
	//print_serial("[EXT2] entry is at inode %d\n", inodeIdx);
	if(inodeIdx == 0){
		print_serial("[EXT2] Adv Listing Failure, invalid inode\n");
		return listing;
	}
	listing.directory_path_len = strlen(path);
	listing.directory_path = strdup(path);
	listing.entries = NULL;
	listing.num_entries = 0;

	struct EXT2_Inode inode = ext2_read_inode_data(ext2, inodeIdx);
	if((inode.type_perms & 0xF000) != EXT2_InodeType_Directory || *((int *) &inode.lsbSize) < 0){
		return listing;
	};

	void *directory_entries = ext2_read_block(ext2, inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;

	//char work_buf[51];
	//memset(work_buf, 0, sizeof(work_buf));

	//ext2_debug_print_inode(&inode);

	int count = 0;
	while(dir->entry_size != 0 && count < 50){
		count++;
		listing.num_entries++;
		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}

	dir = (struct EXT2_Directory *) directory_entries;

	listing.entries = malloc(sizeof(struct DirectoryEntry) * listing.num_entries);
	memset(listing.entries, 0, sizeof(struct DirectoryEntry) * listing.num_entries);
	//print_serial("There are %d entries\n", listing.num_entries);
	
	for(int i = 0; i < listing.num_entries; i++){
		//listing.entries[i].filename = strncpy(dir->name, dir->name_length);
		memset(listing.entries[i].filename, 0, 50);
		memcpy(listing.entries[i].filename, dir->name, dir->name_length);
		
		//print_serial("%d - %s\n", i, listing.entries[i].filename);
		listing.entries[i].name_len = dir->name_length;
		if(dir->type == 2) listing.entries[i].type = ENTRY_DIRECTORY;
		else if(dir->type == 1) listing.entries[i].type = ENTRY_FILE;
		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
	return listing;
}

#include "vfs.h"
int ext2_read(void *f, void *buf, int nbytes){
	struct VFS_File *file = f;
    struct EXT2_FS *ext2 = file->inode.interface->drive->format_info.ext2;
    struct EXT2_Inode *inode = file->inode.fs.fs;
    //print_serial("[VFS] [EXT2] Reading file\n");
    uint32_t block_idx = 0;
    uint32_t single_indirect_idx = 0;
    uint32_t double_indirect_idx = 0;
    uint32_t idx = 0;
    uint32_t block_head = 0;

    uint32_t block_offset = file->head / ext2->block_size;
    uint32_t pointers_per_block = ext2->block_size / 4;

    block_head = file->head % ext2->block_size;

    char *block;
    
    if(block_offset < 12){
        block_idx = block_offset;
        single_indirect_idx = 0;
        double_indirect_idx = 0;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
    }
    else if(12 <= block_offset && block_offset < 12 + pointers_per_block){
        block_idx = 12;
        single_indirect_idx = block_offset - 12;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
    }
    else if(block_offset >= 12 + pointers_per_block){
        block_idx = 13;
        uint32_t offset = block_offset - (11 + (ext2->block_size / 4));
        single_indirect_idx = offset % pointers_per_block - 1;
        double_indirect_idx = (offset / pointers_per_block) * (ext2->block_size / 4) % 255;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        uint32_t *double_indirect_blocks = (uint32_t *) block;
        uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
        
        block = ext2_read_block(ext2, double_indirect_block);
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
    }

    while((uint32_t) block_head < ext2->block_size && idx < (uint32_t) nbytes){
        ((uint8_t *)buf)[idx] = ((uint8_t *)block)[block_head];
        file->head++;
        idx++;
        block_head++;

        if((uint32_t) block_head == ext2->block_size){
            //print_serial("[VFS] [EXT2] Update - Block No. %d Idx: %d\n", file->head / ext2->block_size, idx);
            //print_serial("[VFS] Block offset %d - %d\n", block_idx, inode->BlockPointers[block_idx]);
            if(block_idx == 12){
                handle_first_indirect:;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d, idx %d is %d\n", inode->BlockPointers[block_idx], single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                    if(block_idx == 13){
                        single_indirect_idx = 0;
                    }
                }
                block_head = 0;
            }
            else if(block_idx == 13){
                handle_first_double_indirect:;
                //print_serial("[VFS] [EXT2] Double Indirect Block\n");
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                uint32_t *double_indirect_blocks = (uint32_t *) block;
                uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
                
                block = ext2_read_block(ext2, double_indirect_block);
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d-%d block %d, idx %d is %d\n", inode->BlockPointers[block_idx], double_indirect_idx, double_indirect_block, single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    double_indirect_idx++;
                    single_indirect_idx = 0;
                }

                if(double_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                }

                block_head = 0;
            }
            else if(block_idx > 13){
                //print_serial("[VFS] [EXT2] Fucky Wucky time on the block idx\n");
                break;
            }
            else{
                block_idx++;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                if(block_idx == 12){
                    goto handle_first_indirect;
                }
                if(block_idx == 13){
                    single_indirect_idx = 0;
                    goto handle_first_double_indirect;
                }
            }

            block_head = 0;
        }
    }

    return idx;
}

int ext2_write(void *f, void *buf, int nbytes){
	struct VFS_File *file = f;
    //print_serial("[VFS] Writing %d bytes\n", nbytes);
    struct EXT2_FS *ext2 = file->inode.interface->drive->format_info.ext2;
    struct EXT2_Inode *inode = file->inode.fs.fs;

    if((uint32_t)(file->head + nbytes) >= inode->lsbSize){
        ext2_extendFile(ext2, file->inode.ext2_inode_idx, file->head + nbytes - inode->lsbSize);
    }
    
    uint32_t block_idx = 0;
    uint32_t single_indirect_idx = 0;
    uint32_t double_indirect_idx = 0;
    uint32_t idx = 0;
    uint32_t block_head = 0;

    uint32_t block_offset = file->head / ext2->block_size;
    uint32_t pointers_per_block = ext2->block_size / 4;
    uint32_t last_real_block_idx = 0;

    block_head = file->head % ext2->block_size;

    char *block;
    
    if(block_offset < 12){
        block_idx = block_offset;
        single_indirect_idx = 0;
        double_indirect_idx = 0;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        last_real_block_idx = inode->BlockPointers[block_idx];
    }
    else if(12 <= block_offset && block_offset < 12 + pointers_per_block){
        block_idx = 12;
        single_indirect_idx = block_offset - 12;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        last_real_block_idx = inode->BlockPointers[block_idx];
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
        last_real_block_idx = indirect_block;
    }
    else if(block_offset >= 12 + pointers_per_block){
        block_idx = 13;
        uint32_t offset = block_offset - (11 + (ext2->block_size / 4));
        single_indirect_idx = offset % pointers_per_block - 1;
        double_indirect_idx = (offset / pointers_per_block) * (ext2->block_size / 4) % 255;
        block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
        last_real_block_idx = inode->BlockPointers[block_idx];
        uint32_t *double_indirect_blocks = (uint32_t *) block;
        uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
        
        block = ext2_read_block(ext2, double_indirect_block);
        last_real_block_idx = double_indirect_block;
        uint32_t *indirect_blocks = (uint32_t *) block;
        uint32_t indirect_block = indirect_blocks[single_indirect_idx++];
        block = ext2_read_block(ext2, indirect_block);
        last_real_block_idx = indirect_block;
    }

    while((uint32_t) block_head < ext2->block_size && idx < (uint32_t) nbytes){
        ((uint8_t *)block)[block_head] = ((uint8_t *)buf)[idx];
        //print_serial("%d,%d - %c @ %d\n", block_head, idx, block[block_head], last_real_block_idx);
        file->head++;
        idx++;
        block_head++;

        if((uint32_t) block_head == ext2->block_size){
            ext2_write_block(ext2, last_real_block_idx, block);
            //print_serial("[VFS] [EXT2] Update - Block No. %d Idx: %d\n", file->head / ext2->block_size, idx);
            //print_serial("[VFS] Block offset %d - %d\n", block_idx, inode->BlockPointers[block_idx]);
            if(block_idx == 12){
                handle_first_indirect:;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                last_real_block_idx = inode->BlockPointers[block_idx];
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d, idx %d is %d\n", inode->BlockPointers[block_idx], single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                last_real_block_idx = indirect_block;
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                    if(block_idx == 13){
                        single_indirect_idx = 0;
                    }
                }
                block_head = 0;
            }
            else if(block_idx == 13){
                handle_first_double_indirect:;
                //print_serial("[VFS] [EXT2] Double Indirect Block\n");
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                last_real_block_idx = inode->BlockPointers[block_idx];
                uint32_t *double_indirect_blocks = (uint32_t *) block;
                uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
                
                block = ext2_read_block(ext2, double_indirect_block);
                last_real_block_idx = double_indirect_block;
                uint32_t *indirect_blocks = (uint32_t *) block;
                uint32_t indirect_block = indirect_blocks[single_indirect_idx];
                //print_serial("[VFS] Indirect Block is at %d-%d block %d, idx %d is %d\n", inode->BlockPointers[block_idx], double_indirect_idx, double_indirect_block, single_indirect_idx, indirect_block);
                block = ext2_read_block(ext2, indirect_block);
                last_real_block_idx = indirect_block;
                //block = ext2_read_block(ext2, inode->BlockPointers[0]);
                single_indirect_idx++;
                if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    double_indirect_idx++;
                    single_indirect_idx = 0;
                }

                if(double_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
                    block_idx++;
                }

                block_head = 0;
            }
            else if(block_idx > 13){
                print_serial("[VFS] [EXT2] Fucky Wucky time on the block idx\n");
                break;
            }
            else{
                block_idx++;
                block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
                last_real_block_idx = inode->BlockPointers[block_idx];
                if(block_idx == 12){
                    goto handle_first_indirect;
                }
                if(block_idx == 13){
                    single_indirect_idx = 0;
                    goto handle_first_double_indirect;
                }
            }

            block_head = 0;
        }
    }
    ext2_write_block(ext2, last_real_block_idx, block);

    return idx;
}

void *ext2_getInode(void *fs, char *path){
	struct EXT2_FS *ext2 = fs;
	print_serial("[EXT2] Looking for %s\n", path);
	uint32_t inodeIdx = ext2_get_inodeIdx_from_path(ext2, path);
	if(inodeIdx == 0){
		//print_serial("[VFS] Inode is Zero???\n");
		return NULL;
	}
	print_serial("[EXT2] Found at inode %d\n", inodeIdx);
	//int ext2_inode_idx = inodeIdx;
	struct EXT2_Inode *inode = malloc(sizeof(struct EXT2_Inode));
	*inode = ext2_read_inode_data(ext2, inodeIdx);
	return inode;
}

int ext2_seek(void *f, int offset, int whence){
	struct VFS_File *file_idx = f;
	if(whence == 0){//SEEK_SET
        file_idx->head = offset;
    }
    else if(whence == 1){//SEEK_CUR
        file_idx->head += offset;
    }
    else if(whence == 2){//SEEK_END
        file_idx->head += ((struct EXT2_Inode *) file_idx->inode.fs.fs)->lsbSize + offset;
    }
    else{
        return -1;
    }
	return file_idx->head;
}

void *ext2_generateVFSRoot(struct EXT2_FS *ext2){
	print_serial("[EXT2] Generating Interface for VFS\n");
	struct VFS_RootInterface *interface = malloc(sizeof(struct VFS_RootInterface));
	interface->drive = ext2->drive;
	interface->vfsLetter = 0;
	interface->fs_label = "EXT2";
	interface->root = ext2;

	interface->fs_getLink = ext2_getInode;
	interface->fs_read = ext2_read;
	interface->fs_write = ext2_write;
	interface->fs_seek = ext2_seek;
	interface->fs_creat = ext2_createFile;
	interface->fs_listDirectory = ext2_advListDirectory;
	interface->fs_truncate = NULL;
	return interface;
}
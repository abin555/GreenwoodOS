#include "ext2.h"

void *ext2_read_block(struct EXT2_FS *ext2, uint32_t block_id){
	print_serial("[EXT2] Reading block %d\n", block_id);
	if(block_id > ext2->block_count) return NULL;
	int err = drive_read(ext2->drive, ext2->buf, block_id*ext2->sectors_per_block, ext2->sectors_per_block);
	if(err == -1) return NULL;
	return ext2->buf;
}

struct EXT2_Inode ext2_read_inode_data(struct EXT2_FS *ext2, uint32_t inodeIdx){
	uint32_t group = (inodeIdx - 1) / ext2->inodes_per_group;
	uint32_t inode_table_block = ext2->inode_table_starting_addr[group];
	uint32_t idx_in_group = inodeIdx - group * ext2->inodes_per_group;
	uint32_t block_offset = (idx_in_group - 1) * ext2->inode_size / ext2->block_size;
	uint32_t offset_in_block = (idx_in_group - 1) - block_offset * (ext2->block_size / ext2->inode_size);
	char *block = ext2_read_block(ext2, inode_table_block + block_offset);
	struct EXT2_Inode *inode = ((struct EXT2_Inode *) (block + offset_in_block * ext2->inode_size));
	print_serial("[EXT2] Inode %d: Group %d, Type: 0x%x, Size: %d, Block: %d\n", inodeIdx, group, inode->type_perms & 0xF000, inode->lsbSize, inode_table_block + block_offset);
	return *inode;
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
	print_serial("[EXT2] Looking for %s from inode %d\n", name, parent_inodeIdx);
	struct EXT2_Inode parent_inode = ext2_read_inode_data(ext2, parent_inodeIdx);
	void *directory_entries = ext2_read_block(ext2, parent_inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;

	char work_buf[20];

	while(dir->entry_size != 0){
		memset(work_buf, 0, sizeof(work_buf));
		memcpy(work_buf, dir->name, dir->name_length);
		//print_serial("[EXT2] Inode %d Type %d - ", dir->inode, dir->type);

		if(strcmp(name, work_buf) == 0){
			return dir->inode;
		}

		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
	return 0;
}


void ext2_console_printDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, uint32_t inodeIdx){
	struct EXT2_Inode inode = ext2_read_inode_data(ext2, inodeIdx);
	if((inode.type_perms & 0xF000) != EXT2_InodeType_Directory){
		print_console(console, "File of size %d\n", inode.lsbSize);
		return;
	};
	
	void *directory_entries = ext2_read_block(ext2, inode.BlockPointers[0]);
	struct EXT2_Directory *dir = (struct EXT2_Directory *) directory_entries;

	char work_buf[20];
	//memset(work_buf, 0, sizeof(work_buf));

	while(dir->entry_size != 0){
		memset(work_buf, 0, sizeof(work_buf));
		memcpy(work_buf, dir->name, dir->name_length);
		//for(uint8_t i = 0; i < dir->name_length; i++){
		//	print_serial("%c", dir->name[i]);
		//}
		//print_serial("\n");
		print_console(console, "%s\n", work_buf);

		dir = (struct EXT2_Directory *) ((void *) dir + dir->entry_size);
	}
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
	ext2->buf = (char *) malloc(sizeof(char) * ext2->block_size);
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
		//print_serial("[EXT2] Group %d Block Usage Bitmap Addr: %x, Inode Usage Bitmap Addr: %x, Inode Table Addr: %x\n", i, BGD->block_usage_addr, BGD->inode_usage_addr, BGD->inode_table_addr);

		BGD = BGD + 1;
	}

	//struct EXT2_Inode root_inode = ext2_read_inode_data(ext2, 2);

	//print_serial("[EXT2] Root Directory Block %d\n", root_inode.BlockPointers[0]);
	//ext2_print_directory(ext2, EXT2_ROOT_INODE);

	//print_serial("[EXT2] %d\n", ext2_get_direntry_inode(ext2, EXT2_ROOT_INODE, "LISP"));
	//ext2_print_directory(ext2, ext2_get_direntry_inode(ext2, ext2_get_direntry_inode(ext2, EXT2_ROOT_INODE, "OS"), "term"));
	//struct EXT2_Inode test = ext2_read_inode_data(ext2, ext2_get_direntry_inode(ext2, ext2_get_direntry_inode(ext2, ext2_get_direntry_inode(ext2, EXT2_ROOT_INODE, "OS"), "term"), "term.exe"));
	//ext2_debug_print_inode(&test);
	//print_serial("%d\n", ext2_get_inodeIdx_from_path(ext2, "3D"));
	//ext2_debug_print_inode(&root_inode);

	return 1;
}


uint32_t ext2_get_inodeIdx_from_path(struct EXT2_FS *ext2, char *path){
	uint32_t working_idx = EXT2_ROOT_INODE;
	char work_buf[20];
	int path_component_size;
	int iterations = 0;
	while(*path != '\0'){
		memset(work_buf, 0, sizeof(work_buf));
		path_component_size = 0;
		//memcpy(work_buf, dir->name, dir->name_length);
		for(int i = 0; path[i] != '\0' && path[i] != '/' && i < 20; i++){
			work_buf[i] = path[i];
			path_component_size++;
		}
		working_idx = ext2_get_direntry_inode(ext2, working_idx, work_buf);
		print_serial("[EXT2] Solving Path Iter %d Partial Path: %s Working Inode Idx: %d\n", iterations, work_buf, working_idx);
		path += path_component_size + 1;
		iterations++;
	}
	return working_idx;
}


void ext2_listDirectory(struct CONSOLE *console, struct EXT2_FS *ext2, char *path){
	//print_console(console, "Listing Drive %c path %s\n", ext2->drive->identity, path);
	uint32_t inodeIdx = ext2_get_inodeIdx_from_path(ext2, path);
	print_serial("[EXT2] entry is at inode %d\n", inodeIdx);
	ext2_console_printDirectory(console, ext2, inodeIdx);
}
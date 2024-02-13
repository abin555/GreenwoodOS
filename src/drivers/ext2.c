#include "ext2.h"

void *ext2_read_block(struct EXT2_FS *ext2, uint32_t block_id){
	if(block_id > ext2->block_count) return NULL;
	int err = drive_read(ext2->drive, ext2->buf, block_id*ext2->sectors_per_block, ext2->sectors_per_block);
	if(err == -1) return NULL;
	return ext2->buf;
}

uint32_t ext2_getInodeBlockGroup(struct EXT2_FS *ext2, uint32_t inode){
	return (inode - 1) / ext2->inodes_per_group;
}
uint32_t ext2_getInodeIndexInGroup(struct EXT2_FS *ext2, uint32_t inode){
	return (inode - 1) % ext2->inodes_per_group;
}
uint32_t ext2_getInodeBlock(struct EXT2_FS *ext2, uint32_t inode){
	return (ext2_getInodeIndexInGroup(ext2, inode) * ext2->inode_size) / ext2->block_size;
}

void ext2_printInode(struct EXT2_FS *ext2, uint32_t inode){
	uint32_t block = ext2_getInodeBlock(ext2, inode);
	struct EXT2_Inode *inode_entry = ((struct EXT2_Inode *) ext2_read_block(ext2, block)) + inode;
	print_serial("[EXT2] Inode %d is at block %d and is type: 0x%x\n", inode, block, inode_entry->type_perms & 0xF000);
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
	ext2->inodes_per_group = super_block->inodes_per_group;
	ext2->ext2_version = super_block->rev_level;

	if(ext2->ext2_version < 1){
		ext2->inode_size = 128;
	}
	else{
		ext2->inode_size = super_block->inode_size;
	}

	print_serial("[EXT2] Block Size: %d, Block Count: %d, Inode Count: %d, Starting Block Number: %d, Blocks Per Group: %d, First Inode: %d\n", 
	ext2->block_size, ext2->block_count, ext2->inode_count, ext2->starting_block_number, super_block->blocks_per_group, super_block->first_ino);
	print_serial("[EXT2] Inodes Per Group: %d\n", ext2->inodes_per_group);

	struct EXT2_BlockGroupDescriptor *BGD = ext2_read_block(ext2, ext2->starting_block_number+1);
	ext2->block_addr_block_usage_map = BGD->block_usage_addr;
	ext2->block_addr_inode_usage_map = BGD->inode_usage_addr;
	ext2->inode_table_starting_addr = BGD->inode_table_addr;
	print_serial("[EXT2] Block Usage Bitmap Addr: %d, Inode Usage Bitmap Addr: %d, Inode Table Addr: %d\n", BGD->block_usage_addr, BGD->inode_usage_addr, BGD->inode_table_addr);
	ext2_printInode(ext2, 2);
	return 1;
}
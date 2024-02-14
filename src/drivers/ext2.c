#include "ext2.h"

void *ext2_read_block(struct EXT2_FS *ext2, uint32_t block_id){
	if(block_id > ext2->block_count) return NULL;
	int err = drive_read(ext2->drive, ext2->buf, block_id*ext2->sectors_per_block, ext2->sectors_per_block);
	if(err == -1) return NULL;
	return ext2->buf;
}

struct EXT2_Inode ext2_read_inode_data(struct EXT2_FS *ext2, uint32_t inodeIdx){
	uint32_t group = inodeIdx / ext2->inodes_per_group;
	uint32_t inode_table_block = ext2->inode_table_starting_addr;
	uint32_t idx_in_group = inodeIdx - group * ext2->inodes_per_group;
	uint32_t block_offset = (idx_in_group - 1) * ext2->inode_size / ext2->block_size;
	uint32_t offset_in_block = (idx_in_group - 1) - block_offset * (ext2->block_size / ext2->inode_size);
	char *block = ext2_read_block(ext2, inode_table_block + block_offset);
	struct EXT2_Inode *inode = ((struct EXT2_Inode *) (block + offset_in_block * ext2->inode_size));
	print_serial("[EXT2] Inode %d: Type: 0x%x, Size: %d, Block: %d\n", inodeIdx, inode->type_perms & 0xF000, inode->lsbSize, inode_table_block + block_offset);
	for(int i = 0; i < 15; i++){
		print_serial("[EXT2] Inode %d: DBP %d - %d\n", inodeIdx, i, inode->BlockPointers[i]);
	}
	return *inode;
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
	ext2->group_count = 1 + (super_block->blocks_count-1) / super_block->blocks_per_group;
	ext2->first_inode = super_block->first_ino;

	if(ext2->ext2_version < 1){
		ext2->inode_size = 128;
	}
	else{
		ext2->inode_size = super_block->inode_size;
	}

	print_serial("[EXT2] Block Size: %d, Block Count: %d, Inode Count: %d, Starting Block Number: %d, Blocks Per Group: %d, First Inode: %d, Inodes Per Group: %d\n", 
	ext2->block_size, ext2->block_count, ext2->inode_count, ext2->starting_block_number, super_block->blocks_per_group, ext2->first_inode, ext2->inodes_per_group);
	print_serial("[EXT2] Inodes Per Group: %d\n", ext2->inodes_per_group);

	struct EXT2_BlockGroupDescriptor *BGD = ext2_read_block(ext2, ext2->starting_block_number+1);
	ext2->block_addr_block_usage_map = BGD->block_usage_addr;
	ext2->block_addr_inode_usage_map = BGD->inode_usage_addr;
	ext2->inode_table_starting_addr = BGD->inode_table_addr;
	print_serial("[EXT2] Block Usage Bitmap Addr: %d, Inode Usage Bitmap Addr: %d, Inode Table Addr: %d\n", BGD->block_usage_addr, BGD->inode_usage_addr, BGD->inode_table_addr);

	struct EXT2_Inode root_inode = ext2_read_inode_data(ext2, 2);

	print_serial("[EXT2] Root Directory Block %d\n", root_inode.BlockPointers[0]);

	return 1;
}
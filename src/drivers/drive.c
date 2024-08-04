#include "drive.h"

int drive_count;
struct DRIVE **drives;
char *drive_sector_buf;

void init_drive_system(){
	print_serial("[DRIVE SYS] INIT\n");
	drives = malloc(sizeof(struct DRIVE **) * DRIVE_NUM);
	drive_sector_buf = malloc(512);
	drive_count = 0;
}

struct DRIVE *drive_add(int type, void *driver){
	struct DRIVE *new_drive = malloc(sizeof(struct DRIVE));
	drives[drive_count] = new_drive;
	new_drive->type = type;
	new_drive->identity = 'A' + drive_count;
	new_drive->locked = 0;
	new_drive->ready = 1;
	switch(new_drive->type){
		case Drive_AHCI:
			new_drive->driver.ahci = (struct HBA_PORT *) driver;
			print_serial("[DRIVE SYS] Added new AHCI drive %c:\n", new_drive->identity);
			break;
	}
	
	drive_count++;
	return new_drive;
}

int drive_read(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors){
	if(drive == NULL || buf == NULL)
		return -1;
	
	if(drive->locked) print_serial("[DRIVE SYS] Drive %c is locked\n", drive->identity);
	while(drive->locked || !drive->ready);

	drive->locked = 1;

	int err = -1;

	switch(drive->type){
		case Drive_AHCI:
			//print_serial("[DRIVE SYS] Reading Drive %c at sector %d for %d sectors\n", drive->identity, sector, count_sectors);
			err = AHCI_read((volatile HBA_PORT *) drive->driver.ahci, sector, 0, count_sectors, (uint16_t *) buf);
			while(((volatile HBA_PORT *) drive->driver.ahci)->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)){}
		break;
	}



	drive->locked = 0;
	return err;
}

int drive_write(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors){
	if(drive == NULL || buf == NULL)
		return -1;

	if(drive->locked) print_serial("[DRIVE SYS] Drive %c is locked\n", drive->identity);
	while(drive->locked || !drive->ready);

	drive->locked = 1;
	int err = -1;

	switch(drive->type){
		case Drive_AHCI:
			err = AHCI_write((volatile HBA_PORT *) drive->driver.ahci, sector, 0, count_sectors, (uint32_t *) buf);
			while(((volatile HBA_PORT *) drive->driver.ahci)->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)){}
		break;
	}

	drive->locked = 0;
	return err;
}

struct DRIVE *drive_get(char identity){
	struct DRIVE *drive;
	for(int i = 0; i < drive_count; i++){
		drive = drives[i];
		if(drive->identity == identity) return drive;
	}
	return NULL;
}

void drive_get_format(struct DRIVE *drive){
	print_serial("[DRIVE SYS] Checking Format\n");
	if(FAT_check_format(drive)){
		drive->format = FAT32;
		return;
	}
	else if(ISO9660_check_format(drive)){
		drive->format = ISO9660;
		return;
	}
	else if(ext2_check_format(drive)){
		drive->format = EXT2;
		return;
	}

	drive->format = RAW;
	return;
}

void drive_enumerate(){
	print_serial("[DRIVE SYS] Enumerating Drives\n");
	for(int i = 0; i < drive_count; i++){
		drive_get_format(drives[i]);
	}
}

struct FILE files[NUM_FILES] = {0};

void expandPath(char *dest, int dest_size, struct DIRECTORY *dir, char *path){
	memset(dest, 0, dest_size);
	int idx = 0;
	int path_idx = 0;
	if(path[0] == '/'){
		while(path[idx+1] != 0){
			dest[idx] = path[idx+1];
			idx++;
		}
		return;
	}
	while(dir->path[idx] != 0){
		dest[idx] = dir->path[idx];
		idx++;
	}
	while(path[path_idx] != 0){
		dest[idx+path_idx] = path[path_idx];
		path_idx++;
	}
}

struct FILE *fopen_rel(struct DIRECTORY *dir, char *path){
	char big_path[200];
	memset(big_path, 0, sizeof(big_path));
	expandPath(big_path, sizeof(big_path), dir, path);
	print_serial("[Drive] Opening %s\n", big_path);
	return fopen(big_path);
}

struct FILE *fopen(char *path){
	struct FILE *file = NULL;
	for(int i = 0; i < NUM_FILES; i++){
		if(files[i].info.drive == NULL){
			file = &files[i];
			break;
		}
	}
	if(file == NULL) return NULL;
	if(path == NULL) return NULL;
	if(path[0] == '\0') return NULL;
	
	char drive_letter = path[0];
	path+=2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL) return file;
	if(drive->format == ISO9660){
		file->info = ISO9660_GetFile(drive->format_info.ISO, path);
		//file->info.type = ISO9660;
		if(file->info.drive == NULL || file->info.sector == 0 || file->info.size == 0) return NULL;
		file->head = 0;
		return file;
	}
	if(drive->format == EXT2){
		print_serial("[EXT2] Opening File %s\n", path);
		uint32_t inodeIdx = ext2_get_inodeIdx_from_path(drive->format_info.ext2, path);
		if(inodeIdx == 0) return NULL;
		file->info.drive = drive;
		file->info.inode = (struct EXT2_Inode *) malloc(sizeof(struct EXT2_Inode));
		*file->info.inode = ext2_read_inode_data(drive->format_info.ext2, inodeIdx);
		//file->info.type = EXT2;
		file->info.size = file->info.inode->lsbSize;
		file->info.inodeIdx = inodeIdx;
		file->head = 0;
		return file;
	}

	return NULL;
}

void fclose(struct FILE *file){
	//memset(file, 0, sizeof(struct FILE));
	file->info.drive = NULL;
}

char fgetc(struct FILE *file){
	char *buf;
	if(file->info.drive->format == ISO9660){
		buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector);
		return buf[file->head++];
	}
	return 0;
}

void fputc(struct FILE *file, char c){
	char *buf;
	if(file->info.drive->format == ISO9660){
		buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector);
		buf[file->head] = c;
		ISO_write_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector);
	}
	else if(file->info.drive->format == EXT2){
		//print_serial("[DRIVE] PUTC @ %d -> %c\n", file->head, c);
		struct EXT2_FS *ext2 = file->info.drive->format_info.ext2;
		uint32_t block_idx = file->head / ext2->block_size;
		if(block_idx < 12){
			uint32_t block = file->info.inode->BlockPointers[block_idx];
			buf = ext2_read_block(ext2, block);
			buf[file->head % ext2->block_size] = c;
			ext2_write_block(ext2, block, buf);
		}
	}
}

int fsize(struct FILE *file){
	print_serial("[DRIVE] File size is %d\n", file->info.size);
	return file->info.size;
}

int fseek(struct FILE *file, int idx){
	file->head = idx;
	return file->head;
}

int fcopy(struct FILE *file, char *buf, int buf_size){
	print_serial("[DRIVE] Copying file to 0x%x\n", (uint32_t) buf);
	if(file == NULL || buf_size == 0) return 1;
	if(file->info.drive->format == ISO9660){
		char *iso_buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector);
		if(iso_buf == NULL) return 1;
		int idx = 0;
		int sector_offset = 0;
		while(file->head < 512*4 && idx < buf_size){
			buf[idx] = iso_buf[file->head];
			file->head++;
			idx++;
			if(file->head == 512*4){
				sector_offset++;
				print_serial("[DRIVE] Sector Offset %d\n", sector_offset);
				iso_buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector+sector_offset);
				file->head = 0;
			}
		}

		return 0;
	}
	else if(file->info.drive->format == EXT2){
		print_serial("[DRIVE] [EXT2] Copying File, buf size is %d\n", buf_size);
		struct EXT2_FS *ext2 = file->info.drive->format_info.ext2;
		struct EXT2_Inode *inode = file->info.inode;
		//char *block_buf = (char *) ext2_read_block(ext2, inode)
		uint32_t block_idx = 0;
		uint32_t single_indirect_idx = 0;
		uint32_t double_indirect_idx = 0;
		uint32_t idx = 0;
		char *block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);

		while((uint32_t) file->head < ext2->block_size && idx <= (uint32_t) buf_size){
			buf[idx] = block[file->head];
			//print_serial("%c\n", buf[idx]);
			file->head++;
			idx++;

			if((uint32_t) file->head == ext2->block_size){
				//print_serial("[DRIVE] Block offset %d - %d\n", block_idx, inode->BlockPointers[block_idx]);
				if(block_idx == 12){
					handle_first_indirect:;
					block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
					uint32_t *indirect_blocks = (uint32_t *) block;
					uint32_t indirect_block = indirect_blocks[single_indirect_idx];
					//print_serial("[DRIVE] Indirect Block is at %d, idx %d is %d\n", inode->BlockPointers[block_idx], single_indirect_idx, indirect_block);
					block = ext2_read_block(ext2, indirect_block);
					//block = ext2_read_block(ext2, inode->BlockPointers[0]);
					single_indirect_idx++;
					if(single_indirect_idx * sizeof(uint32_t) >= ext2->block_size){
						block_idx++;
						if(block_idx == 13){
							single_indirect_idx = 0;
						}
					}
					file->head = 0;
				}
				else if(block_idx == 13){
					handle_first_double_indirect:;
					//print_serial("[DRIVE] [EXT2] Double Indirect Block\n");
					block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
					uint32_t *double_indirect_blocks = (uint32_t *) block;
					uint32_t double_indirect_block = double_indirect_blocks[double_indirect_idx];
					
					block = ext2_read_block(ext2, double_indirect_block);
					uint32_t *indirect_blocks = (uint32_t *) block;
					uint32_t indirect_block = indirect_blocks[single_indirect_idx];
					//print_serial("[DRIVE] Indirect Block is at %d-%d block %d, idx %d is %d\n", inode->BlockPointers[block_idx], double_indirect_idx, double_indirect_block, single_indirect_idx, indirect_block);
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

					file->head = 0;
				}
				else if(block_idx > 13){
					print_serial("[DRIVE] [EXT2] Fucky Wucky time on the block idx\n");
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

				file->head = 0;
			}
		}
		print_serial("[DRIVE][EXT2] Copied %d bytes\n", idx);
		return 0;
	}
	return 1;
}

int fwrite(struct FILE *file, char *buf, uint32_t numBytes){
	print_serial("[DRIVE] Saving file from 0x%x of %d bytes\n", (uint32_t) buf, numBytes);
	if(file == NULL || numBytes == 0) return 1;
	if(file->info.drive->format == ISO9660){
		return 1;
	}
	else if(file->info.drive->format == EXT2){
		print_serial("[DRIVE] [EXT2] Writing File, buf size is %d\n", numBytes);
		struct EXT2_FS *ext2 = file->info.drive->format_info.ext2;
		struct EXT2_Inode *inode = file->info.inode;
		//char *block_buf = (char *) ext2_read_block(ext2, inode)
		uint32_t block_idx = 0;
		uint32_t single_indirect_idx = 0;
		uint32_t idx = 0;
		char *block;// = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
		ext2_write_block(ext2, inode->BlockPointers[block_idx], buf + (idx / ext2->block_size));
		uint32_t head = file->head;
		while((uint32_t) head < ext2->block_size && idx <= (uint32_t) numBytes){
			//buf[idx] = block[head];
			//print_serial("%c\n", buf[idx]);
			head++;
			idx++;

			if((uint32_t) head == ext2->block_size){
				print_serial("[DRIVE] Block offset %d - %d\n", block_idx, inode->BlockPointers[block_idx]);
				if(block_idx == 12){
					handle_first_indirect:;
					block = ext2_read_block(ext2, inode->BlockPointers[block_idx]);
					//ext2_write_block(ext2, inode->BlockPointers[block_idx], buf + (idx / ext2->block_size));
					uint32_t *indirect_blocks = (uint32_t *) block;
					uint32_t indirect_block = indirect_blocks[single_indirect_idx];
					print_serial("[DRIVE] Indirect Block is at %d, idx %d is %d\n", inode->BlockPointers[block_idx], single_indirect_idx, indirect_block);
					//block = ext2_read_block(ext2, indirect_block);
					ext2_write_block(ext2, indirect_block, buf + (idx / ext2->block_size));
					//block = ext2_read_block(ext2, inode->BlockPointers[0]);
					single_indirect_idx++;
					if(single_indirect_idx * sizeof(uint32_t) > ext2->block_size){
						block_idx++;
					}
					head = 0;
				}
				else if(block_idx == 13){
					print_serial("[DRIVE] [EXT2] Double Indirect Block\n");
					break;
				}
				else if(block_idx > 13){
					print_serial("[DRIVE] [EXT2] Fucky Wucky time on the block idx\n");
					break;
				}
				else{
					block_idx++;
					ext2_write_block(ext2, inode->BlockPointers[block_idx], buf + (idx / ext2->block_size));
					if(block_idx == 12){
						goto handle_first_indirect;
					}
				}

				head = 0;
			}
		}
		print_serial("[DRIVE][EXT2] Wrote %d bytes\n", idx);
		return 0;
	}
	return 1;
}

int fexists(char *path){
	char drive_letter = path[0];
	path+=2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL) return 0;
	if(drive->format == ISO9660){
		return ISO9660_checkExists(drive->format_info.ISO, path);
	}
	else if(drive->format == EXT2){
		uint32_t inode = ext2_get_inodeIdx_from_path(drive->format_info.ext2, path);
		if(inode) return 1;
		return 0;
	}
	return 0;
}

int fexists_rel(struct DIRECTORY *dir, char *path){
	char big_path[100];
	expandPath(big_path, sizeof(big_path), dir, path);
	char drive_letter = big_path[0];
	path = big_path + 2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL) return 0;
	if(drive->format == ISO9660){
		return ISO9660_checkExists(drive->format_info.ISO, path);
	}
	else if(drive->format == EXT2){
		uint32_t inode = ext2_get_inodeIdx_from_path(drive->format_info.ext2, path);
		if(inode) return 1;
		return 0;
	}
	return 0;
}

int changeDirectory(struct DIRECTORY *dir, char *path){
	/*
	Manipulates the task currentDirectory string.
	Everything is appended to the string except for:
	./ is ignored, and removed.
	../ removed until the previous / and continues
	- Note, ensure that ../ to idx 0 is covered.
	*/
	int path_size = 0;
	//char workbuf[100];
	while(path[path_size] != 0){
		path_size++;
	}
	if(path[0] == '/'){//Absolute Directory
		if(fexists(path+1)){
			memset(dir->path, 0 , sizeof(dir->path));
			memcpy(dir->path, path+1, path_size-1);
			if(dir->path[path_size-2] != '/') dir->path[path_size-1] = '/';
			print_serial("Path is now %s\n", dir->path);
			return 0;
		}
		else{
			return 1;
		}
	}
	else{//Relative Directory
		
	}
	return 1;
}

void listFiles(struct CONSOLE *console, struct DIRECTORY *dir, char *path){
	char big_path[100];
	expandPath(big_path, sizeof(big_path), dir, path);
	char drive_letter = big_path[0];
	path = big_path + 2;
	struct DRIVE *drive = drive_get(drive_letter);
	print_serial("[DRIVE] Directory Listing %s / %s\n", big_path, path);
	if(drive->format == ISO9660){
		ISO9660_printFileList(console, drive->format_info.ISO, path);
	}
	else if(drive->format == EXT2){
		ext2_listDirectory(console, drive->format_info.ext2, path);
	}
}

int fmkdir(struct DIRECTORY *dir, char *path){
	char big_path[100];
	expandPath(big_path, sizeof(big_path), dir, path);
	print_serial("[Drive] Mkdir %s\n", big_path);
	char drive_letter = big_path[0];
	path = big_path+2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL){
		print_serial("[Drive] Drive access to %c failed?\n", drive_letter);
		return 1;
	}
	if(drive->format == ISO9660){
		return ISO9660_createDirectory(drive->format_info.ISO, path);
	}
	else if(drive->format == EXT2){
		return ext2_createDirectory(drive->format_info.ext2, path);
	}
	return 1;
}

int fmkfile(struct DIRECTORY *dir, char *path, int size){
	char big_path[100];
	expandPath(big_path, sizeof(big_path), dir, path);
	print_serial("[Drive] Make File %s\n", big_path);
	char drive_letter = big_path[0];
	path = big_path+2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL) return 1;
	if(drive->format == ISO9660){
		return ISO9660_createFile(drive->format_info.ISO, path, size);
	}
	else if(drive->format == EXT2){
		return ext2_createFile(drive->format_info.ext2, path, size);
	}
	return 1;
}

int fextend(struct FILE *file, uint32_t extendAmount){
	print_serial("[DRIVE] Extending File by %d bytes\n", extendAmount);
	if(file == NULL) return 1;
	struct DRIVE *drive = file->info.drive;
	if(drive->format == ISO9660){
		return 1;
	}
	else if(drive->format == EXT2){
		print_serial("[DRIVE] Current size is %d\n", file->info.inode->lsbSize);
		if(!ext2_extendFile(drive->format_info.ext2, file->info.inodeIdx, extendAmount)){
			print_serial("[DRIVE] EXT2 Successfully Extended file, reflecting in FILE structure\n");
			file->info.size += extendAmount;
			*file->info.inode = ext2_read_inode_data(drive->format_info.ext2, file->info.inodeIdx);
			print_serial("[DRIVE] New size is %d (%d)\n", file->info.inode->lsbSize, file->info.size);
			return 0;
		}
	}
	return 1;
}
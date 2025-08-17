#include "drive.h"

int drive_count;
struct DRIVE **drives;
char *drive_sector_buf;

void init_drive_system(){
	print_serial("[DRIVE SYS] INIT\n");
	drives = malloc(sizeof(struct DRIVE **) * DRIVE_NUM);
	drive_sector_buf = malloc(512);
	memset(drive_sector_buf, 0, 512);
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
	
	//if(drive->locked) print_serial("[DRIVE SYS] Drive %c is locked\n", drive->identity);
	//while(drive->locked || !drive->ready);

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

	//if(drive->locked) print_serial("[DRIVE SYS] Drive %c is locked\n", drive->identity);
	//while(drive->locked || !drive->ready);

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
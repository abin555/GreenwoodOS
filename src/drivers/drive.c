#include "drive.h"

int drive_count;
struct DRIVE **drives;

void init_drive_system(){
	drives = malloc(sizeof(struct DRIVE **) * DRIVE_NUM);
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


int drive_read(struct DRIVE *drive, char *buf, uint64_t sector, uint32_t count_sectors){
	if(drive == NULL || buf == NULL)
		return -1;
	
	while(drive->locked || !drive->ready);

	drive->locked = 1;

	int err = -1;

	switch(drive->type){
		case Drive_AHCI:
			err = AHCI_read((volatile HBA_PORT *) drive->driver.ahci, (sector && 0xFFFFFFFF), (sector >> 32), count_sectors, (uint16_t *) buf);
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
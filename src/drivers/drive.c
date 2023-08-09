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
	switch(new_drive->type){
		case Drive_AHCI:
			new_drive->driver.ahci = (struct HBA_PORT *) driver;
			print_serial("[DRIVE SYS] Added new AHCI drive %c:\n", new_drive->identity);
			break;
	}
	
	drive_count++;
	return new_drive;
}

/*
int drive_read(struct DRIVE *drive, char *buf, uint64_t sector, uint32_t count_sectors){

}
*/
#ifndef DRIVE_H
#define DRIVE_H

//#include "stdlib.h"
#include "allocator.h"
#include "serial.h"
#include "ahci.h"
#include "FAT32.h"
#include "ISO9660.h"


#define DRIVE_NUM 20

struct DRIVE{
	enum{
		Drive_AHCI
	} type;
	union {
		struct HBA_PORT *ahci;
	} driver;

	enum{
		FAT32,
		FAT16,
		FAT12,
		exFAT,
		ISO9660,
		RAW
	} format;
	union{
		struct FAT32 *fat32;
		struct ISO9660 *ISO;
	} format_info;
	bool locked;
	bool ready;
	char identity;
};

struct File{
	struct DRIVE *drive;
	uint32_t sector;
	uint32_t size;
};

extern struct DRIVE **drives;
extern int drive_count;
extern char *drive_sector_buf;

void init_drive_system();
void drive_enumerate();

void drive_get_format(struct DRIVE *drive);

struct DRIVE *drive_add(int type, void *driver);

struct DRIVE *drive_get(char identity);

int drive_read(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);

#endif
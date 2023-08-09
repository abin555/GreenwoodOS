#ifndef DRIVE_H
#define DRIVE_H

//#include "stdlib.h"
#include "allocator.h"
#include "serial.h"
#include "ahci.h"


#define DRIVE_NUM 20

struct DRIVE{
	enum{
		Drive_AHCI
	} type;
	union {
		struct HBA_PORT *ahci;
	} driver;

	bool locked;
	bool ready;
	char identity;
};

extern struct DRIVE **drives;
extern int drive_count;

void init_drive_system();

struct DRIVE *drive_add(int type, void *driver);

struct DRIVE *drive_get(char identity);

int drive_read(struct DRIVE *drive, char *buf, uint64_t sector, uint32_t count_sectors);

#endif
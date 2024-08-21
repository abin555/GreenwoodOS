#ifndef DRIVE_H
#define DRIVE_H

//#include "stdlib.h"
#include "allocator.h"
#include "serial.h"
#include "ahci.h"
#include "FAT32.h"
#include "ISO9660.h"
#include "ext2.h"
#include "console.h"

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
		RAW,
		EXT2
	} format;
	union{
		struct FAT32 *fat32;
		struct ISO9660 *ISO;
		struct EXT2_FS *ext2;
	} format_info;
	bool locked;
	bool ready;
	char identity;
};

struct DIRECTORY{
	char path[100];
};

struct File_Info{
	struct DRIVE *drive;
	struct EXT2_Inode *inode;
	uint32_t inodeIdx;
	uint32_t sector;
	uint32_t size;
};

struct FILE{
	struct File_Info info;
	int head;
};

enum DirEntryType{
	ENTRY_FILE = 0,
	ENTRY_DIRECTORY = 1
};

struct DirectoryEntry{
	char filename[50];
	int name_len;
	uint32_t type;
};

struct DirectoryListing{
	char *directory_path;
	int directory_path_len;
	struct DirectoryEntry *entries;
	int num_entries;
};

#define NUM_FILES 50
extern struct FILE files[NUM_FILES];

extern struct DRIVE **drives;
extern int drive_count;
extern char *drive_sector_buf;

void init_drive_system();
void drive_enumerate();

void drive_get_format(struct DRIVE *drive);

struct DRIVE *drive_add(int type, void *driver);

struct DRIVE *drive_get(char identity);

int drive_read(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);
int drive_write(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);

struct FILE *fopen_rel(struct DIRECTORY *dir, char *path);
struct FILE *fopen(char *path);
void fclose(struct FILE *file);
char fgetc(struct FILE *file);
void fputc(struct FILE *file, char c);
int fsize(struct FILE *file);
int fseek(struct FILE *file, int idx);
int fcopy(struct FILE *file, char *buf, int buf_size);
int fwrite(struct FILE *file, char *buf, uint32_t numBytes);
int fexists(char *path);
int fexists_rel(struct DIRECTORY *dir, char *path);
int changeDirectory(struct DIRECTORY *dir, char *path);
void listFiles(struct CONSOLE *console, struct DIRECTORY *dir, char *path);
int fmkdir(struct DIRECTORY *dir, char *path);
int fmkfile(struct DIRECTORY *dir, char *path, int size);
int fextend(struct FILE *file, uint32_t extendAmount);

struct DirectoryListing listDirectory(struct DIRECTORY *dir, char *path); 
struct DirectoryListing task_listDirectory(char *path);
#endif

#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "ISO9660.h"
#include "memory.h"
#include "drivers.h"
#include "console.h"
#include "stdint.h"

typedef enum{
    FS_UNDEF_Device = 0,
    FS_SATA_Device = 1,
    FS_USB_Device = 2,
    FS_SATAPI_Device = 3
}FilesystemDriverTypes;

struct __filesystemDrive;
typedef struct __filesytemDrive{
    uint8_t type;
    uint8_t FS_type;
    char name[16];
    uint32_t *portData;
    void (*read)(uint32_t *, uint32_t, uint32_t, uint32_t, uint32_t *);
    void (*write)(uint32_t *, uint32_t, uint32_t, uint32_t, uint32_t *);
}filesystemDrive;

filesystemDrive **FileSystems;
uint32_t numFS;

uint8_t *filesystem_default_read_buffer;

void init_filesystem();

void addFileSystemDevice(unsigned int deviceType, char *name, uint32_t *portStruct, void *read, void *write);
void FS_read(uint32_t drive, uint32_t sector, uint32_t countSectors, uint32_t *buffer);
void FS_write(uint32_t drive, uint32_t sector, uint32_t countSectors, uint32_t *buffer);

struct Internal_FILE{
    uint8_t drive;
    uint32_t sector;
    uint32_t sector_count;
    uint32_t size;
};

typedef struct FILE{
    uint8_t drive;
    uint32_t sector;
    uint32_t sector_count;
    uint32_t size;
} FILE;

FILE* fopen(int drive, char* filename);
int fclose(FILE* file);
uint8_t *fread(int drive, uint32_t sector);

#endif
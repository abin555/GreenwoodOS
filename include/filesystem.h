#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memory.h"
#include "drivers.h"
#include "console.h"
#include "stdint.h"

typedef enum{
    FS_UNDEF_Device = 0,
    FS_SATA_Device = 1,
    FS_USB_Device = 2
}FilesystemDriverTypes;

struct __filesystemDrive;
typedef struct __filesytemDrive{
    uint8_t type;
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

#endif
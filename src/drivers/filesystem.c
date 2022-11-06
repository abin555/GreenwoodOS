#include "filesystem.h"

void init_filesystem(){
    printk("Filesystem Init\n");
    FileSystems = (filesystemDrive **)malloc(32);
    filesystem_default_read_buffer = (uint8_t *) malloc(512);
    numFS = 0;
}

void addFileSystemDevice(unsigned int deviceType, char *name, uint32_t *portStruct, void *read, void *write){
    printk("Added File System Type: %2h ID: %2h\n", deviceType, numFS);
    FileSystems[numFS] = malloc(sizeof(filesystemDrive));
    FileSystems[numFS]->type = deviceType;
    FileSystems[numFS]->portData = portStruct;
    FileSystems[numFS]->read = read;
    FileSystems[numFS]->write = write;
    memcpy(name, FileSystems[numFS]->name, 16);
    numFS++;
}

void FS_read(uint32_t drive, uint32_t sector, uint32_t countSectors, uint32_t *buffer){
    printk("[FS] Reading\n");
    if(drive < numFS){
        FileSystems[drive]->read(
            FileSystems[drive]->portData,
            sector,
            0,
            countSectors,
            buffer
        );
    }
}

void FS_write(uint32_t drive, uint32_t sector, uint32_t countSectors, uint32_t *buffer){
    printk("[FS] Writing\n");
    if(drive < numFS){
        FileSystems[drive]->write(
            FileSystems[drive]->portData,
            sector,
            0,
            countSectors,
            buffer
        );
    }
}
#include "filesystem.h"

void init_filesystem(){
    printk("Filesystem Init\n");
    FileSystems = (filesystemDrive **)malloc(32 * sizeof(filesystemDrive));
    numFS = 0;
}

void addFileSystemDevice(unsigned int deviceType, char *name, uint32_t *portStruct, void *read, void *write){
    printk("Added File System ID: %2h\n", deviceType);
    FileSystems[numFS] = malloc(sizeof(filesystemDrive));
    FileSystems[numFS]->type = deviceType;
    FileSystems[numFS]->portData = portStruct;
    FileSystems[numFS]->read = read;
    FileSystems[numFS]->write = write;
    strcpy(name, FileSystems[numFS]->name, 16);
    numFS++;
}

void FS_read(uint32_t drive, uint32_t sector, uint32_t countSectors, uint16_t *buffer){
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
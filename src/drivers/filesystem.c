#include "filesystem.h"
uint32_t active_directory;
uint8_t active_drive;

void init_filesystem(){
    printk("Filesystem Init\n");
    FileSystems = (filesystemDrive **)malloc(32);
    filesystem_default_read_buffer = (uint8_t *) malloc(512);
    numFS = 0;
    active_directory = 0;
    active_drive = 0;
}

void ready_filesystem(){
    for(uint32_t i = 0; i < numFS; i++){
        if(ISO_checkFS(i)){
            ISO_read_volume_descriptor(i);
        }
    }
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
    //printk("[FS] Reading Drive %2x Sector %x to %x\n", drive, sector, (uint32_t) buffer);
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
    //printk("[FS] Writing Drive %2x Sector %x from %x\n", drive, sector, (uint32_t) buffer);
    if(drive < numFS){
        if(!FileSystems[drive]->write) return;
        FileSystems[drive]->write(
            FileSystems[drive]->portData,
            sector,
            0,
            countSectors,
            buffer
        );
    }
}

FILE* fopen(char* filename){
    struct Internal_FILE Ifile;
    FILE* file;
    
    if(ISO_checkFS(active_drive)){
        Ifile = ISO_open_file(active_drive, active_directory, filename);
        file = (FILE*) malloc(sizeof(FILE));
        file->drive = Ifile.drive;
        file->sector = Ifile.sector;
        file->sector_count = Ifile.sector_count;
        file->size = Ifile.size;
    }
    return file;
}

uint8_t *fread(int drive, uint32_t sector){
    if(ISO_checkFS(drive)){
        ISO_read_sector(drive, sector);
        return ISO9660_sector_buffer;
    }
    return 0;
}

int fclose(FILE* file){
    free(file);
    return 1;
}
uint32_t num_fs_entries = 0;
int add_FS_Item(uint8_t type, uint8_t drive, uint32_t sector, uint32_t parent_item_entry, uint32_t size, uint16_t sector_count, uint8_t FS_Type, char name[20]){
    int start_index = num_fs_entries;
    FS_entries[num_fs_entries].type = type;
    FS_entries[num_fs_entries].drive = drive;
    FS_entries[num_fs_entries].sector = sector;
    FS_entries[num_fs_entries].parent_item_entry = parent_item_entry;
    FS_entries[num_fs_entries].size = size;
    FS_entries[num_fs_entries].sector_count = sector_count;
    FS_entries[num_fs_entries].FS_type = FS_Type;
    //memcpy(ISO_FS[num_fs_entries].name, name, 20);
    memcpy(name, FS_entries[num_fs_entries].name, 20);

    num_fs_entries++;
    return start_index;
}
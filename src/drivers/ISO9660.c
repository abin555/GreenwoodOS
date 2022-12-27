#include "ISO9660.H"

uint8_t ISO9660_sector_buffer[0x800];
uint32_t ISO9660_drive_root_file;

uint8_t* ISO_read_sector(int drive, int sector){
    FS_read(drive, sector*4, 4, (uint32_t*) &ISO9660_sector_buffer);
    return ISO9660_sector_buffer;
}

void read_directory(int drive, int parent_FS_index, int directory_sector){
    struct ISO_Directory_Entry *root_dir = (struct ISO_Directory_Entry*) ISO_read_sector(drive, directory_sector);   
    char name[20];
    while(root_dir->length){
        memset(name, 0, 20);
        if(root_dir->length <= 0x22){
            root_dir = (struct ISO_Directory_Entry *)((uint8_t *)(root_dir)+root_dir->length);
            continue;
        }
        uint8_t isFolder = 1;
        for(int i = 0; i < root_dir->name_len; i++){
            if(root_dir->name[i] != ';'){
                name[i] = root_dir->name[i];
            }
            else{
                isFolder = 0;
                break;
            }
        }
        
        uint32_t sector = *((uint32_t *) root_dir->sector.le);
        //uint32_t size = *((uint32_t *) root_dir->size.le) / 0x800 + 1;
        uint32_t size = *((uint32_t *) root_dir->size.le);
        uint16_t sector_size = *((uint32_t *) root_dir->size.le) / 0x800 + 1;

        root_dir = (struct ISO_Directory_Entry *)((uint8_t *)(root_dir)+root_dir->length);

        if(!isFolder){
            add_FS_Item(File, drive, sector, parent_FS_index, size, sector_size, ISO, name);
        }
    }
}

void read_path(int drive, int path_sector, int path_size){
    int index = 0;
    uint8_t* path_mem_sector = ISO_read_sector(drive, path_sector);
    char name[20];
    while(index < path_size){
        memset(name, 0, 20);
        ISO_read_sector(drive, path_sector);
        
        uint8_t Name_Length = ((uint8_t *) path_mem_sector)[index+0];
        
        uint32_t Path_Entry_Length = 8;
        Path_Entry_Length += Name_Length;
        if(Name_Length % 2){
            Path_Entry_Length += 1;
        }
        struct ISO_PathTable_Entry *pathtable = (struct ISO_PathTable_Entry *) (((uint32_t) path_mem_sector)+index);
        
        if(index == 0){
            ISO9660_drive_root_file = num_fs_entries;
            name[0] = 'A'+drive;
            name[1] = ':';
        }
        else{
            for(uint32_t i = 0; i < Name_Length; i++){
                name[i] = pathtable->name[i];
            }
        }
        uint32_t sector = *((uint32_t*) (path_mem_sector+index+2));
        
        int parent;

        if(index == 0){
            parent = add_FS_Item(Folder, drive, sector, 0, 1, 1, ISO, name);
        }
        else{
            parent = add_FS_Item(Folder, drive, sector, ISO9660_drive_root_file+*((uint8_t*) (path_mem_sector+index+5)), 1, 1, ISO, name);
        }
        
        read_directory(drive, parent, sector);
        index+=Path_Entry_Length;
    }
}

void ISO_list_files(){
    printk("\nFiles:\n");
    for(uint32_t i = 0; i < num_fs_entries; i++){
        if(FS_entries[i].type == 1){
            printk("File: %s Parent Folder: %s Size: %x Sectors: %x", FS_entries[i].name, FS_entries[FS_entries[i].parent_item_entry].name, FS_entries[i].size, FS_entries[i].sector_count);
        }
        else if(FS_entries[i].type == 2){
            printk("Folder: %s Parent Folder: %s Size: %x Sectors: %x", FS_entries[i].name, FS_entries[FS_entries[i].parent_item_entry].name, FS_entries[i].size, FS_entries[i].sector_count);
        }
        printk(" Sector: %x Drive: %x\n", FS_entries[i].sector, FS_entries[i].drive);
    }
}

struct Internal_FILE ISO_open_file(int drive, int active_directory, char *filename){
    printk("\nOpening File: %s\n", filename);
    struct Internal_FILE output = {
        0,
        0,
        0,
        0
    };
    uint8_t fileFound = 0;
    uint32_t fileSector = 0;
    uint32_t fileSize = 0;
    uint16_t fileSectorCount = 0;
    int index = 0;
    int directory_count = 0;
    while(filename[index] != 0){
        if(filename[index] == '/'){
            directory_count++;
        }
        index++;
    }
    //printf("%d Directories to trace\n", directory_count);
    int scan_index = 0;
    uint32_t parent = active_directory;
    for(int i = 0; i < directory_count; i++){
        char name[20];
        memset(name, 0, 20);
        int name_index = 0;
        while(filename[scan_index] != '/' && filename[scan_index] != 0){
            name[name_index] = filename[scan_index];
            name_index++;
            scan_index++;
        }
        scan_index++;
        //printf("Partial Name %s\n", name);

        for(uint32_t file = 0; file < num_fs_entries; file++){
            if(!check_str_equ(name, FS_entries[file].name)){
                //printf("Name Matches %d\n", file);
                if(FS_entries[file].parent_item_entry == parent){
                    parent = file;
                    //printf("Matching Parent!\n");
                }
            }
        }
    }
    //printf("Parent Index: %d Parent Name: %s\n", parent, FS_entries[parent].name);
    for(uint32_t file = 0; file < num_fs_entries; file++){
        if(!check_str_equ(filename+scan_index, FS_entries[file].name)){
            //printf("Name Matches %d\n", file);
            if(FS_entries[file].parent_item_entry == parent){
                parent = file;
                //printf("Matching File!\n");
                fileFound = 1;
                fileSector = FS_entries[file].sector;
                fileSize = FS_entries[file].size;
                fileSectorCount = FS_entries[file].sector_count;
            }
        }
    }

    if(fileFound){
        output.drive = drive;
        output.sector = fileSector;
        output.sector_count = fileSectorCount;
        output.size = fileSize;
    }
    return output;
}

void ISO_read_volume_descriptor(int drive){
    printk("Loading ISO9660 File System from drive %x\n", drive);
    struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) (ISO_read_sector(drive, 0x10));

    //uint32_t *directory_entry = (uint32_t *) (ISO_read_sector(drive, 0x10) + 0x9E);

    read_path(drive, *((uint32_t *) primary_vol->path_table_le.le), *((uint32_t*)(primary_vol->path_table_size.le)));

    //ISO_list_files();
}

int ISO_checkFS(int drive){
    struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) (ISO_read_sector(drive, 0x10));
    if(
        primary_vol->hdr.magic[0] == 'C' &&
        primary_vol->hdr.magic[1] == 'D' &&
        primary_vol->hdr.magic[2] == '0' &&
        primary_vol->hdr.magic[3] == '0' &&
        primary_vol->hdr.magic[4] == '1'
    ){
        printk("Drive %x Is ISO Format: %s\n", drive, primary_vol->hdr.magic);
        return 1;
    }
    else{
        printk("Drive %x Is not ISO Format %s\n", drive, primary_vol->hdr.magic);
        return 0;
    }
}
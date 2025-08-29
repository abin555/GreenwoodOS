#include <sys/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/task.h>

#define getLe32(x) *((uint32_t *) &x.le)
#define getLe16(x) *((uint16_t *) &x.le)

typedef struct { uint16_t le; }__attribute__((packed))          l9660_luint16;
typedef struct { uint16_t be; }__attribute__((packed))          l9660_buint16;
typedef struct { uint8_t le[2], be[2]; }__attribute__((packed))   l9660_duint16;
typedef struct { uint8_t le[4]; }__attribute__((packed))          l9660_luint32;
typedef struct { uint8_t be[4]; }__attribute__((packed))          l9660_buint32;
typedef struct { uint8_t le[4], be[4]; }__attribute__((packed))   l9660_duint32;

struct ISO_Descriptor_Time{
    char d[17];
}__attribute__((packed));
struct ISO_File_Time{
    char d[7];
}__attribute__((packed));

struct ISO_PathTable_Entry{
    uint8_t Directory_Name_Length;
    uint8_t extended_attribute_length;
    uint32_t Location_of_Extent;
    uint16_t Directory_Number_Parent_Directory;
    char name[];
}__attribute__((packed));

struct ISO_Directory_Entry{
    uint8_t         length;
    uint8_t         xattr_length;
    l9660_duint32   sector;
    l9660_duint32   size;
    struct ISO_File_Time  time;
    uint8_t         flags;
    uint8_t         unit_size;
    uint8_t         gap_size;
    l9660_duint16   vol_seq_number;
    uint8_t         name_len;
    char            name[0];
}__attribute__((packed));

struct ISO_vdesc_header{
    uint8_t type;
    char magic[5];
    uint8_t version;
}__attribute__((packed));

struct ISO_Primary_Volume_Descriptor{
    struct ISO_vdesc_header  hdr;
    char                pad0[1];
    char                system_id[32];
    char                volume_id[32];
    char                pad1[8];
    l9660_duint32       volume_space_size;
    char                pad2[32];
    l9660_duint16       volume_set_size;
    l9660_duint16       volume_seq_number;
    l9660_duint16       logical_block_size;
    l9660_duint32       path_table_size;
    l9660_luint32       path_table_le;
    l9660_luint32       path_table_opt_le;
    l9660_buint32       path_table_be;
    l9660_buint32       path_table_opt_be;
    union {
        struct ISO_Directory_Entry root_dir_ent;
        char            pad3[34];
    };
    char                volume_set_id[128];
    char                data_preparer_id[128];
    char                app_id[128];
    char                copyright_file[38];
    char                abstract_file[36];
    char                bibliography_file[37];
    struct ISO_Descriptor_Time volume_created;
    struct ISO_Descriptor_Time volume_modified;
    struct ISO_Descriptor_Time volume_expires;
    struct ISO_Descriptor_Time volume_effective;
    uint8_t             file_structure_version;
    char                pad4[1];
    char                app_reserved[512];
    char                reserved[653];
}__attribute__((packed));

union ISO_Volume_Descriptor{
    struct ISO_vdesc_header hdr;
    char _bits[2048];
}__attribute__((packed));

struct ISO9660;

struct DRIVE {
	enum{
		Drive_AHCI
	} type;
	union {
		void *ahci;
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
		void *fat32;
		struct ISO9660 *ISO;
		void *ext2;
	} format_info;
	char locked;
	char ready;
	char identity;
};

struct ISO9660{
	struct DRIVE *drive;
	char *buf;
	//struct ISO9660_FS_Entry root;
    uint32_t root_directory_sector;
    uint32_t nextTableSector;
    uint32_t nextFileSector;
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

struct VFS_RootInterface {
    struct DRIVE *drive;
    char vfsLetter;
    char *fs_label;
    void *root;

    void *(*fs_getLink)(void *, char *path, uint32_t *meta);
    int (*fs_read)(void *f, void *buf, int nbytes);
    int (*fs_write)(void *f, void *buf, int nbytes);
    int (*fs_seek)(void *f, int offset, int whence);
    int (*fs_creat)(void *fs, char *path, unsigned int size);
    int (*fs_creatDir)(void *fs, char *path);
    struct DirectoryListing (*fs_listDirectory)(void *fs, char *path);
    int (*fs_truncate)(void *f, unsigned int len);
};

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
} __attribute__((packed));

struct DRIVE **drives;
int *drive_count;
int (*drive_read)(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);
int (*drive_write)(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);
void (*vfs_addFS)(struct VFS_RootInterface *interface);
void *(*kmalloc)(size_t size);


uint8_t *sector_buf;

uint8_t *ISO_read_sector(struct DRIVE *drive, char *buf, int sector){
	if(sector < 0) return NULL;
	drive_read(drive, (char *) buf, sector*4, 4);
	return (uint8_t *) buf;
}

void ISO_write_sector(struct DRIVE *drive, char *buf, int sector){
	drive_write(drive, (char *) buf, sector*4, 4);
}

int check_iso(struct DRIVE *drive){
    printf("[ISO] Checking format on drive %c\n", drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) (ISO_read_sector(drive, (char *) sector_buf, 0x10));
    if(
        primary_vol->hdr.magic[0] == 'C' &&
        primary_vol->hdr.magic[1] == 'D' &&
        primary_vol->hdr.magic[2] == '0' &&
        primary_vol->hdr.magic[3] == '0' &&
        primary_vol->hdr.magic[4] == '1'
    ){
        printf("[ISO] Drive %c Type: %d Is ISO Format: ", drive->identity, primary_vol->hdr.type);
		for(int i = 0; i < 5; i++){
			printf("%c", primary_vol->hdr.magic[i]);
		}
		printf("\n");
        /*
		struct ISO9660 *fmt = (struct ISO9660 *) malloc(sizeof(struct ISO9660));
		if(fmt == NULL){
			print_serial("[ISO] Error Getting Memory!\n");
			return 0;
		}
        */
        return 1;
    }
    else{
        return 0;
    }
}

void setup_iso(int drive_id){
    task_lock(1);
    struct DRIVE *drive = drives[drive_id];
    check_iso(drive);
    
    //struct ISO9660 *fmt = drives[drive_id]->format_info.ISO;
    //printf("Root Directory Sector: 0x%x\nNext Table Sector: 0x%x\n Next File Sector: 0x%x\n", fmt->root_directory_sector, fmt->nextTableSector, fmt->nextFileSector);
    

    task_lock(0);
}

int main(int argc, char **argv){
    FILE *kernel_fn_file = fopen("/-/dev/kernel", "r");
    if(kernel_fn_file != NULL){
        int num_fns;
        fread(&num_fns, sizeof(num_fns), 1, kernel_fn_file);
        struct kernel_fn_def *def;
        for(int i = 0; i < num_fns; i++){
            fread(&def, sizeof(def), 1, kernel_fn_file);
            printf("#%d: 0x%x - \"%s\"\n", i, def->fn, def->name);
            if(!strcmp("vfs_addFS", def->name)){
                vfs_addFS = def->fn;
                printf("Found vfs_addFS!\n");
            }
            if(!strcmp("drive_read", def->name)){
                drive_read = def->fn;
                printf("Found drive_read!\n");
            }
            if(!strcmp("drive_write", def->name)){
                drive_write = def->fn;
                printf("Found drive_write!\n");
            }
            if(!strcmp("drives_s", def->name)){
                drives = def->fn;
                printf("Found drives_s @ 0x%x!\n", (uint32_t) drives);
            }
            if(!strcmp("drive_count_s", def->name)){
                drive_count = def->fn;
                printf("Found drive_count!\n");
            }
            if(!strcmp("kmalloc", def->name)){
                kmalloc = def->fn;
                printf("Found kmalloc!\n");
            }
        }
    }
    fclose(kernel_fn_file);
    if(
        vfs_addFS == NULL ||
        drive_read == NULL ||
        drive_write == NULL ||
        drives == NULL ||
        drive_count == NULL ||
        kmalloc == NULL
    ){
        printf("Critical Error, undefined symbols!\n");
        return 1;
    }

    sector_buf = memory_requestRegion(512*4*sizeof(uint8_t));

    printf("There are %d drives!\n", *drive_count);
    for(int i = 0; i < *drive_count; i++){
        switch(drives[i]->format){
            case EXT2:
                printf("Drive %d (%c) is EXT2\n", i, drives[i]->identity);
                break;
            case ISO9660:
                printf("Drive %d (%c) is ISO9660 (Target)\n", i, drives[i]->identity);
                setup_iso(i);
                break;
        }
    }

    return 0;
}
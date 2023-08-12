#ifndef ISO9660_H
#define ISO9660_H

#include "drive.h"
#include "allocator.h"
#include "serial.h"

struct DRIVE;

#define ISO9660_SECTOR_SIZE_BYTES 512*4

uint8_t ISO9660_sector_buffer[ISO9660_SECTOR_SIZE_BYTES];

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
    char            name[/*name_len*/];
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

struct ISO9660_FS_Entry{
	char name[20];
	enum{
		File = 1,
		Folder = 2,
        Root = 3,
        Special = 4
	} type;
	uint32_t sector;
	uint32_t size;
	uint32_t sector_count;
    int num_children;
    struct ISO9660_FS_Entry *parent;
	struct ISO9660_FS_Entry *children;
};

struct ISO9660{
	struct DRIVE *drive;
	char *buf;
	//struct ISO9660_FS_Entry root;
    uint32_t root_directory_sector;
};


uint8_t *ISO_read_sector(struct DRIVE *drive, char *buf, int sector);

int ISO9660_check_format(struct DRIVE *drive);
void ISO9660_read_volume(struct ISO9660 *iso);
void ISO9660_print_tree(struct ISO9660_FS_Entry *fs);
uint32_t ISO9660_getDirectorySector(struct ISO9660 *iso, uint32_t dir_sector, char *folder);
struct File_Info ISO9660_GetFile(struct ISO9660 *iso, char *path);
void ISO9660_printTree(struct ISO9660 *iso);
int ISO9660_openFileName(struct ISO9660 *iso, char *name, char *buf, int buf_size);
int ISO9660_openFile(struct ISO9660 *iso, struct File_Info file, char *buf, int buf_size);

#endif
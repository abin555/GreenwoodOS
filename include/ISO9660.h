#ifndef ISO9660_H
#define ISO9660_H
#include <stdint.h>
#include "memory.h"
#include "console.h"
#include "filesystem.h"
#include "utilities.h"

extern uint8_t ISO9660_sector_buffer[0x800];

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





uint8_t* ISO_read_sector(int drive, int sector);
void ISO_list_files();
struct Internal_FILE ISO_open_file(int drive, int active_directory, char *filename);
void ISO_read_volume_descriptor(int drive);
int ISO_checkFS(int drive);

#endif
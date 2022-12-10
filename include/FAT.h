#ifndef FAT_H
#define FAT_H

#include "filesystem.h"

struct FAT32_Boot_Record{
    uint8_t jmp_info[3];
    char OEM_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_FATs;
    uint16_t num_dir_entry;
    uint16_t sectors_in_volume;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads_per_storage;
    uint32_t num_hidden_sectors;
    uint32_t large_sectors;

    uint32_t FAT_size;
    uint16_t flags;
    uint8_t signature;
    uint16_t FAT_version;
    uint32_t cluster_num_root;
    uint32_t cluster_num_fsinfo;
    uint32_t cluster_num_backup;
    uint8_t reserved[12];
    uint8_t drive_num;
    uint8_t NT_flags;
    uint8_t signature_2;
    uint32_t volumeID;
    char Volume_Label[11];
    char Sys_Identify[8];
};

#endif
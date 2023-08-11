#ifndef FAT32_H
#define FAT32_H

#include "drive.h"
#include "allocator.h"
#include "serial.h"

struct DRIVE;

struct fat_extBS_32
{
	//extended fat32 stuff
	unsigned int		table_size_32;
	unsigned short		extended_flags;
	unsigned short		fat_version;
	unsigned int		root_cluster;
	unsigned short		fat_info;
	unsigned short		backup_BS_sector;
	unsigned char 		reserved_0[12];
	unsigned char		drive_number;
	unsigned char 		reserved_1;
	unsigned char		boot_signature;
	unsigned int 		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];
 
}__attribute__((packed));
 
struct fat_extBS_16
{
	//extended fat12 and fat16 stuff
	unsigned char		bios_drive_num;
	unsigned char		reserved1;
	unsigned char		boot_signature;
	unsigned int		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];
 
}__attribute__((packed));
 
struct fat_BS
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	    bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char		extended_section[54];
 
}__attribute__((packed));


struct FAT32{
	uint8_t *buffer;
	struct DRIVE *drive;
	uint16_t bytes_per_sector;
	uint8_t  sectors_per_cluster;
	uint16_t num_reserved;
	uint8_t  num_FAT;
	uint16_t num_root_entries;
	uint32_t total_sectors;
	uint32_t fat_size;
	uint32_t root_dir_sectors;
	uint32_t first_data_sector;
	uint32_t first_fat_sector;
	uint32_t data_sectors;
	uint32_t total_clusters;
	uint32_t first_root_dir_sector_num;
};

struct FAT32_dir_ent{
	char name[11];
	uint8_t attributes;
	uint8_t NTRes;
	uint8_t time_created_tenth;
	uint16_t time_created;
	uint16_t date_created;
	uint16_t date_last_access;
	uint16_t first_cluster_high;
	uint16_t time_write;
	uint16_t date_write;
	uint16_t first_cluster_low;
	uint32_t file_size;
}__attribute__((packed));

struct FAT_dir{
	uint32_t sector;
	uint32_t cluster;
	uint32_t rw_offset;

	uint32_t start_sector;
	uint32_t size;

	struct FAT32 *volume;
};

struct FAT_file{
	uint32_t sector;
	uint32_t cluster;
	uint32_t rw_offset;
	uint32_t size;
	uint32_t start_sector;
	uint32_t global_offset;
	
	struct FAT32 *volume;
};

int FAT_check_format(struct DRIVE *drive);
void fat32_print_dir_sector_info(struct DRIVE *drive, uint32_t sector);

#endif
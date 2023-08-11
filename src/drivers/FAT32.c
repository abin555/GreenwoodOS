#include "FAT32.h"

int FAT_check_format(struct DRIVE *drive){
	if(drive == NULL){
		return 0;
	}
	print_serial("[FAT] Checking Drive %c\n", drive->identity);
	int err = drive_read(drive, drive_sector_buf, 0, 1);
	if(!err) return 0;
	print_serial("[FAT] - Boot: ");
	for(int i = 0; i < 3; i++){
		print_serial("%x ", drive_sector_buf[i]);
	}
	print_serial("\n");
	if(!((uint8_t) drive_sector_buf[0] == 0xEB && (uint8_t)drive_sector_buf[2] == 0x90)) return 0;
	print_serial("[FAT] Identity: ");
	for(int i = 0; i < 8; i++){
		print_serial("%c", drive_sector_buf[0x03+i]);
	}
	print_serial("\n");

	struct fat_BS *fat = (struct fat_BS *) drive_sector_buf;
	struct fat_extBS_32 *fat_ext_32 = (struct fat_extBS_32 *)(((char *) fat)+0x024);
	//struct fat_extBS_32 *fat_ext_32 = (struct fat_BS *)(((char *) fat)+0x024);
	uint32_t total_sectors = (fat->total_sectors_16 == 0) ? fat->total_sectors_32 : fat->total_sectors_16;
	uint32_t fat_size = (fat->table_size_16 == 0)? fat_ext_32->table_size_32 : fat->table_size_16;
	uint32_t root_dir_sectors = ((fat->root_entry_count * 32) + (fat->bytes_per_sector - 1)) / fat->bytes_per_sector;
	uint32_t first_data_sector = fat->reserved_sector_count + (fat->table_count * fat_size) + root_dir_sectors;
	uint32_t first_fat_sector = fat->reserved_sector_count;
	uint32_t first_root_dir_sector_num = fat->reserved_sector_count + (fat->table_count * fat->table_size_16);
	uint32_t data_sectors = total_sectors - (fat->reserved_sector_count + (fat->table_count * fat_size) + root_dir_sectors);
	uint32_t total_clusters = data_sectors / fat->sectors_per_cluster;
	print_serial("[FAT] Drive %c has %d sectors and %d clusters\n", drive->identity, total_sectors, total_clusters);

	if(total_clusters < 4085){
		drive->format = FAT12;
		print_serial("[FAT] Drive %c is FAT12\n", drive->identity);
		return 0;
	}
	else if(total_clusters < 65525){
		drive->format = FAT16;
		print_serial("[FAT] Drive %c is FAT16\n", drive->identity);
		return 0;
	}
	else{
		drive->format = FAT32;
		drive->format_info.fat32 = (struct FAT32 *) malloc(sizeof(struct FAT32));
		drive->format_info.fat32->buffer = (uint8_t *) malloc(512);
		drive->format_info.fat32->drive = drive;
		drive->format_info.fat32->bytes_per_sector = fat->bytes_per_sector;
		drive->format_info.fat32->sectors_per_cluster = fat->sectors_per_cluster;
		drive->format_info.fat32->num_reserved = fat->reserved_sector_count;
		drive->format_info.fat32->num_FAT = fat->table_count;
		drive->format_info.fat32->num_root_entries = fat->root_entry_count;
		drive->format_info.fat32->total_sectors = total_sectors;
		drive->format_info.fat32->fat_size = fat_size;
		drive->format_info.fat32->root_dir_sectors = root_dir_sectors;
		drive->format_info.fat32->first_data_sector = first_data_sector;
		drive->format_info.fat32->first_fat_sector = first_fat_sector;
		drive->format_info.fat32->data_sectors = data_sectors;
		drive->format_info.fat32->total_clusters = total_clusters;
		drive->format_info.fat32->first_root_dir_sector_num = first_root_dir_sector_num;

		print_serial("[FAT] Drive %c is FAT32\n", drive->identity);
		print_serial("[FAT] Drive %c Info:\n", drive->identity);
		print_serial(" - Root Sector: %d\n - First Data Sector: %d\n", drive->format_info.fat32->first_root_dir_sector_num, drive->format_info.fat32->first_fat_sector);
		fat32_print_dir_sector_info(drive, drive->format_info.fat32->first_fat_sector);
	}
	
	return 1;
}

void fat32_print_dir_sector_info(struct DRIVE *drive, uint32_t sector){
	if(!drive_read(drive, (char *) drive->format_info.fat32->buffer, sector, 1)){
		print_serial("Error!\n");
	}
	struct FAT32_dir_ent *dir = (struct FAT32_dir_ent *) drive->format_info.fat32->buffer;
	print_serial("[FAT32] Drive %c Sector %d Directory Info:\n", drive->identity, sector);
	print_serial(" - Name: ");
	for(int i = 0; i < 11; i++){
		print_serial("%c", dir->name[i]);
	}
	print_serial("\n - Attributes: 0x%x\n", dir->attributes);
	print_serial(" - Date Created: %d\n", dir->date_created);
	print_serial(" - Cluster High: 0x%x\n - Cluster  Low: 0x%x\n", dir->first_cluster_high, dir->first_cluster_low);
}
#include "ISO9660.h"

uint8_t *ISO_read_sector(struct DRIVE *drive, char *buf, int sector){
	drive_read(drive, (char *) buf, sector*4, 4);
	return ISO9660_sector_buffer;
}

int ISO9660_check_format(struct DRIVE *drive){
	print_serial("[ISO] Checking format on drive %c\n", drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) (ISO_read_sector(drive, (char *) ISO9660_sector_buffer, 0x10));
    if(
        primary_vol->hdr.magic[0] == 'C' &&
        primary_vol->hdr.magic[1] == 'D' &&
        primary_vol->hdr.magic[2] == '0' &&
        primary_vol->hdr.magic[3] == '0' &&
        primary_vol->hdr.magic[4] == '1'
    ){
        print_serial("[ISO] Drive %c Is ISO Format: ", drive->identity);
		for(int i = 0; i < 5; i++){
			print_serial("%c", primary_vol->hdr.magic[i]);
		}
		print_serial("\n");

		struct ISO9660 *fmt = (struct ISO9660 *) malloc(sizeof(struct ISO9660));
		if(fmt == NULL){
			print_serial("[ISO] Error Getting Memory!\n");
			return 0;
		}
		drive->format = ISO9660;
		drive->format_info.ISO = fmt;
		fmt->buf = (char *) malloc(ISO9660_SECTOR_SIZE_BYTES);
		fmt->drive = drive;
		fmt->root = NULL;
		ISO9660_read_volume(fmt);
        return 1;
    }
    else{
        return 0;
    }
}

void ISO9660_read_path(struct ISO9660 *iso, int path_sector, int path_size){
	int index = 0;
	uint8_t *path_mem_sector = ISO_read_sector(iso->drive, iso->buf, path_sector);
	char name[20];
	print_serial("[ISO] Drive %c Root has %d children\n", iso->drive->identity, path_size);
	return;
	while(index < path_size){
		memset(name, 0, 20);
		ISO_read_sector(iso->drive, iso->buf, path_sector);

		uint8_t Name_Length = ((uint8_t *) path_mem_sector)[index+0];
		uint32_t Path_Entry_Length = 8;
		Path_Entry_Length += Name_Length;
		if(Name_Length % 2){
			Path_Entry_Length += 1;
		}
		struct ISO_PathTable_Entry *pathtable = (struct ISO_PathTable_Entry *) (((uint32_t) path_mem_sector) + index);
		if(index = 0){
			name[0] = '/';
			name[1] = 0;
		}
		else{
			for(uint32_t i = 0; i < Name_Length; i++){
				name[i] = pathtable->name[i];
			}
		}
		uint32_t sector = *((uint32_t*) (path_mem_sector+index+2));

		int parent;

		if(index == 0){
			
		}
	}
}

void ISO9660_read_volume(struct ISO9660 *iso){
	print_serial("[ISO] Loading ISO9660 File Table from Drive %c\n", iso->drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) (ISO_read_sector(iso->drive, iso->buf, 0x10));
	ISO9660_read_path(iso, *((uint32_t *) primary_vol->path_table_le.le), *((uint32_t*)(primary_vol->path_table_size.le)));
}
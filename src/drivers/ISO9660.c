#include "ISO9660.h"

#define getLe32(x) *((uint32_t *) &x.le)
#define getLe16(x) *((uint16_t *) &x.le)

uint8_t *ISO_read_sector(struct DRIVE *drive, char *buf, int sector){
	drive_read(drive, (char *) buf, sector*4, 4);
	return (uint8_t *) buf;
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
        print_serial("[ISO] Drive %c Type: %d Is ISO Format: ", drive->identity, primary_vol->hdr.type);
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
		ISO9660_read_volume(fmt);
        return 1;
    }
    else{
        return 0;
    }
}

void ISO9660_read_volume(struct ISO9660 *iso){
	print_serial("[ISO] Loading ISO9660 File Table from Drive %c\n", iso->drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) ISO_read_sector(iso->drive, iso->buf, 0x10);
	iso->root_directory_sector = getLe32(primary_vol->root_dir_ent.sector);
}

int strcmp(const char *s1, const char *s2){
	while(*s1 && (*s1 == *s2)){
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char*)s2;
}

void strprnt(char *s, int size){
	for(int i = 0; i < size; i++){
		print_serial("%c", s[i]);
	}
}

//Returns Directory Entry sector that matches Input String
uint32_t ISO9660_getDirectorySector(struct ISO9660 *iso, uint32_t dir_sector, char *folder){
	if(dir_sector == 0){
		return 0;
	}
	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, dir_sector);
	print_serial("[ISO] Looking For %s\n", folder);
	char tempbuf[20];
	while(dir->length != 0){
		memset(tempbuf, 0, sizeof(tempbuf));
		if(dir->name_len == 1 && dir->name[0] == 0){
			tempbuf[0] = '.';
		}
		else if(dir->name_len == 1 && dir->name[0] == 1){
			tempbuf[0] = '.';
			tempbuf[1] = '.';
		}
		else{
			memcpy(tempbuf, dir->name, dir->name_len);
		}		

		print_serial("Entry: %s\n", tempbuf);

		if(!(strcmp(tempbuf, folder))){
			return getLe32(dir->sector);
		}

		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
	return 0;
}

struct File_Info ISO9660_getFile(struct ISO9660 *iso, uint32_t dir_sector, char *file){
	struct File_Info retFile = {0};
	if(dir_sector == 0){
		return retFile;
	}
	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, dir_sector);
	char namebuf[20];
	memset(namebuf, 0, sizeof(namebuf));
	char tempbuf[20];
	int namebuf_idx = 0;
	while(file[namebuf_idx] != 0){
		namebuf[namebuf_idx] = file[namebuf_idx];
		namebuf_idx++;
	}
	namebuf[namebuf_idx] = ';';
	namebuf[namebuf_idx+1] = '1';
	print_serial("[ISO] Looking For %s\n", namebuf);
	while(dir->length != 0){
		memset(tempbuf, 0, sizeof(tempbuf));
		if(dir->name_len == 1 && dir->name[0] == 0){
			tempbuf[0] = '.';
		}
		else if(dir->name_len == 1 && dir->name[0] == 1){
			tempbuf[0] = '.';
			tempbuf[1] = '.';
		}
		else{
			memcpy(tempbuf, dir->name, dir->name_len);
		}	

		if(!strcmp(tempbuf, namebuf)){
			print_serial("Found %s\n", tempbuf);
			goto foundFile;
		}

		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
	return retFile;
	foundFile:
	retFile.drive = iso->drive;
	retFile.sector = getLe32(dir->sector);
	retFile.size = getLe32(dir->size);

	return retFile;
}


struct File_Info ISO9660_GetFile(struct ISO9660 *iso, char *path){
	print_serial("[ISO] Opening %s\n", path);
	struct File_Info retFile = {0};
	char work_buf[100];
	memset(work_buf, 0, 20);
	struct DRIVE *drive = iso->drive;
	int idx = 0;
	int work_idx = 0;
	print_serial("[ISO] Looking for %s on Drive %c\n", path, drive->identity);

	bool isFile = 0;
	uint32_t dirSector = iso->root_directory_sector;
	while(!isFile){
		memset(work_buf, 0, 20);
		while(path[idx] != '\0' && path[idx] != '/'){
			work_buf[work_idx] = path[idx];
			idx++;
			work_idx++;
			if(path[idx] == '\0'){//Last entry on path should be a file, and thus the string is null terminated;
				isFile = 1;
				idx++;
				//print_serial("This is a file %s\n", work_buf);
				goto OpenFile;
			}
		}
		idx++;
		work_idx = 0;
		print_serial("[ISO] Opening Folder %s\n", work_buf);
		dirSector = ISO9660_getDirectorySector(iso, dirSector, work_buf);
	}
	OpenFile:
	print_serial("[ISO] Looking for File %s\n", work_buf);
	retFile = ISO9660_getFile(iso, dirSector, work_buf);
	if(retFile.drive == NULL){
		print_serial("[ISO] Error finding %s\n", path);
		return retFile;
	}
	print_serial("Sector %x Size %d\n", retFile.sector, retFile.size);
	return retFile;
}

void ISO9660_printTree_Recur(struct ISO9660 *iso, uint32_t directory_sector, int layer){
	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, directory_sector);
	char tempbuf[20];
	while(dir->length != 0){
		if(dir->name_len == 1 && (dir->name[0] == 0 || dir->name[0] == 1)){
			goto skip;
		}
		for(int i = 0; i < layer; i++){
			print_serial(" ");
		}
		memset(tempbuf, 0, sizeof(tempbuf));
		memcpy(tempbuf, dir->name, dir->name_len);
		print_serial("%s\n", tempbuf);
		if(tempbuf[dir->name_len - 1] != '1' && tempbuf[dir->name_len - 2] != ';'){
			ISO9660_printTree_Recur(iso, getLe32(dir->sector), layer+1);
			ISO_read_sector(iso->drive, iso->buf, directory_sector);
		}
		skip:
		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
}

void ISO9660_printTree(struct ISO9660 *iso){
	print_serial("ISO9660 File Tree for Drive %c\n", iso->drive->identity);
	print_serial("%c:\n", iso->drive->identity);
	ISO9660_printTree_Recur(iso, iso->root_directory_sector, 1);
}

int ISO9660_openFileName(struct ISO9660 *iso, char *name, char *buf, int buf_size){
	struct File_Info file = ISO9660_GetFile(iso, name);
	if(file.drive == NULL || iso == NULL || buf == NULL){
		return 0;
	}
	ISO_read_sector(iso->drive, iso->buf, file.sector);
	for(uint32_t i = 0; i < file.size && i < (uint32_t) buf_size; i++){
		buf[i] = iso->buf[i];
	}
	return 1;
}

int ISO9660_openFile(struct ISO9660 *iso, struct File_Info file, char *buf, int buf_size){
	if(iso == NULL || file.drive == NULL) return 0;
	ISO_read_sector(iso->drive, iso->buf, file.sector);
	for(uint32_t i = 0; i < file.size && i < (uint32_t) buf_size; i++){
		buf[i] = iso->buf[i];
	}
	return 1;
}
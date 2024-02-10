#include "ISO9660.h"

#define getLe32(x) *((uint32_t *) &x.le)
#define getLe16(x) *((uint16_t *) &x.le)

uint8_t *ISO_read_sector(struct DRIVE *drive, char *buf, int sector){
	if(sector < 0) return NULL;
	drive_read(drive, (char *) buf, sector*4, 4);
	return (uint8_t *) buf;
}

void ISO_write_sector(struct DRIVE *drive, char *buf, int sector){
	drive_write(drive, (char *) buf, sector*4, 4);
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
	iso->nextTableSector = iso->root_directory_sector;
	//ISO9660_printTree(iso);
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
	memset(work_buf, 0, 100);
	struct DRIVE *drive = iso->drive;
	int idx = 0;
	int work_idx = 0;
	print_serial("[ISO] Looking for %s on Drive %c\n", path, drive->identity);

	bool isFile = 0;
	uint32_t dirSector = iso->root_directory_sector;
	while(!isFile && dirSector != 0){
		memset(work_buf, 0, 100);
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
		print_serial("%s @ %d\n", tempbuf, directory_sector);
		if(tempbuf[dir->name_len - 1] != '1' && tempbuf[dir->name_len - 2] != ';'){
			if(getLe32(dir->sector) > iso->nextTableSector) iso->nextTableSector = getLe32(dir->sector) + 1;
			ISO9660_printTree_Recur(iso, getLe32(dir->sector), layer+1);
			ISO_read_sector(iso->drive, iso->buf, directory_sector);
		}
		else{
			if(getLe32(dir->sector) > iso->nextFileSector) iso->nextFileSector = getLe32(dir->sector) + (getLe32(dir->size) / 2048) + 1;
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

int ISO9660_checkExists(struct ISO9660 *iso, char *path){
	print_serial("[ISO] %c Checking if %s exists\n", iso->drive->identity, path);
	char work_buf[20];
	memset(work_buf, 0, sizeof(work_buf));
	int idx = 0;
	int work_idx = 0;
	
	uint32_t dirSector = iso->root_directory_sector;
	uint32_t dirSector_back = 0;
	while(path[idx] != '\0'){
		dirSector_back = dirSector;
		memset(work_buf, 0, sizeof(work_buf));
		while(path[idx] != '\0' && path[idx] != '/'){
			work_buf[work_idx] = path[idx];
			idx++;
			work_idx++;
		}
		print_serial("Checking if folder: %s\n", work_buf);
		if(path[idx] != 0){idx++;}
		dirSector = ISO9660_getDirectorySector(iso, dirSector_back, work_buf);
		if(dirSector != 0){
			print_serial("Folder exists\n");
		}
		if(dirSector == 0){//Check if file
			print_serial("Checking if file %s\n", work_buf);
			struct File_Info fileinfo = ISO9660_getFile(iso, dirSector_back, work_buf);
			if(fileinfo.drive != NULL){
				print_serial("File Exists!\n");
				return 1;
			}
			else{
				return 0;
			}
		}
		print_serial("Dir Sector: %d\n", dirSector);
		work_idx = 0;
	}
	if(dirSector == 0) return 0;
	return 1;
}


void ISO9660_printFileList(struct CONSOLE *console, struct ISO9660 *iso, char *path){
	print_serial("[ISO] Printing Directory Contents: %c/%s\n", iso->drive->identity, path);
	char work_buf[20];
	memset(work_buf, 0, sizeof(work_buf));
	int idx = 0;
	int work_idx = 0;
	
	uint32_t dirSector = iso->root_directory_sector;
	while(path[idx] != '\0'){
		memset(work_buf, 0, sizeof(work_buf));
		while(path[idx] != '\0' && path[idx] != '/'){
			work_buf[work_idx] = path[idx];
			idx++;
			work_idx++;
		}
		if(path[idx] != 0){idx++;}
		dirSector = ISO9660_getDirectorySector(iso, dirSector, work_buf);
		print_serial("[ISO] Dir Sector: %d\n", dirSector);
		work_idx = 0;
	}
	if(dirSector == 0) return;

	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, dirSector);
	memset(work_buf, 0, sizeof(work_buf));	
	while(dir->length != 0){
		if(dir->name_len == 1){
			if(dir->name[0] == 0){
				print_console(console, ".\n");
				goto finish;
			}
			else if(dir->name[0] == 1){
				print_console(console, "..\n");
				goto finish;
			}
			goto finish;
		}
		
		if(dir->name[dir->name_len-2] == ';' && dir->name[dir->name_len-1] == '1') dir->name[dir->name_len-2] = 0;
		memset(work_buf, 0, sizeof(work_buf));
		memcpy(work_buf, dir->name, dir->name_len);
		print_console(console, "%s\n", work_buf);

		finish:
		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
}

int ISO9660_createDirectory(struct ISO9660 *iso, char *path){
	if(ISO9660_checkExists(iso, path)){
		return 1;
	}
	char *newDirName;
	int path_size = 0;
	int i;
	for(i = 0; path[i] != 0; i++){
		path_size++;
	}
	i = path_size;
	print_serial("Path %s Size is %d\n", path, path_size);
	while(path[i] != '/' && i != 0){
		i--;
	}
	newDirName = &path[i+1];
	path[i] = 0;
	print_serial("Creating Directory %s at parent %s for Drive %c\n", newDirName, path, iso->drive->identity);
	
	char work_buf[100];
	bool isDone = 0;
	uint32_t parentSector = iso->root_directory_sector;
	int idx = 0;
	int work_idx = 0;

	while(!isDone && parentSector != 0){
		memset(work_buf, 0, 100);
		while(path[idx] != '\0' && path[idx] != '/'){
			work_buf[work_idx] = path[idx];
			idx++;
			work_idx++;
			if(path+idx+1 >= newDirName){//Pointers will match when this is completed
				isDone = 1;
				idx++;
				break;
			}
		}
		idx++;
		work_idx = 0;
		parentSector = ISO9660_getDirectorySector(iso, parentSector, work_buf);
	}
	print_serial("Found Parent Directory at Sector %d\n", parentSector);
	if(parentSector == 0) return 1;

	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, parentSector);
	while(dir->length != 0){
		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
	//Search for available sector for new directory table.
	print_serial("New table sector is %d\n", iso->nextTableSector);
	uint32_t newTableSector = iso->nextTableSector;
	iso->nextTableSector++;

	//Create Directory Entry & Save.
	print_serial("New directory %s name size is %d\n", newDirName, path_size - (newDirName - path));
	dir->name_len = path_size - (newDirName - path);
	dir->length = sizeof(struct ISO_Directory_Entry) + dir->name_len;
	*((uint16_t *) &dir->sector.le) = newTableSector & 0xFFFF;
	*((uint16_t *) &dir->sector.be) = newTableSector & 0xFFFF0000 >> 16;
	memcpy(dir->name, newDirName, dir->name_len);
	ISO_write_sector(iso->drive, iso->buf, parentSector);
	
	//Create . & .. directories
	dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, newTableSector);
	dir->name_len = 1;
	dir->name[0] = 0;
	dir->length = sizeof(struct ISO_Directory_Entry) + dir->name_len;
	*((uint16_t *) &dir->sector.le) = newTableSector & 0xFFFF;
	*((uint16_t *) &dir->sector.be) = newTableSector & 0xFFFF0000 >> 16;
	dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	dir->name_len = 1;
	dir->name[0] = 1;
	dir->length = sizeof(struct ISO_Directory_Entry) + dir->name_len;
	*((uint16_t *) &dir->sector.le) = parentSector & 0xFFFF;
	*((uint16_t *) &dir->sector.be) = parentSector & 0xFFFF0000 >> 16;
	ISO_write_sector(iso->drive, iso->buf, newTableSector);
	return 0;
}

#include "multitasking.h"

int ISO9660_createFile(struct ISO9660 *iso, char *path, uint32_t size){
	if(ISO9660_checkExists(iso, path)){
		return 1;
	}
	char *newDirName;
	int path_size = 0;
	int i;
	for(i = 0; path[i] != 0; i++){
		path_size++;
	}
	i = path_size;
	print_serial("Path %s Size is %d\n", path, path_size);
	while(path[i] != '/' && i != 0){
		i--;
	}
	newDirName = &path[i+1];
	path[i] = 0;
	print_serial("Creating File %s at parent %s for Drive %c\n", newDirName, path, iso->drive->identity);
	print_console(tasks[task_running_idx].console, "Creating File %s at parent %s for Drive %c\n", newDirName, path, iso->drive->identity);
	char work_buf[100];
	bool isDone = 0;
	uint32_t parentSector = iso->root_directory_sector;
	int idx = 0;
	int work_idx = 0;

	while(!isDone && parentSector != 0){
		memset(work_buf, 0, 100);
		while(path[idx] != '\0' && path[idx] != '/'){
			work_buf[work_idx] = path[idx];
			idx++;
			work_idx++;
			if(path+idx+1 >= newDirName){//Pointers will match when this is completed
				isDone = 1;
				idx++;
				break;
			}
		}
		idx++;
		work_idx = 0;
		parentSector = ISO9660_getDirectorySector(iso, parentSector, work_buf);
	}
	print_serial("Found Parent Directory at Sector %d\n", parentSector);
	print_console(tasks[task_running_idx].console, "Found Parent Directory at Sector %d\n", parentSector);
	if(parentSector == 0) return 1;

	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, parentSector);
	while(dir->length != 0){
		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}

	print_serial("Next file sector is %d\n", iso->nextFileSector);
	int numsectors = size / 2048 + 2;
	int realsize = numsectors * 2048;
	uint32_t newTableSector = iso->nextFileSector;
	iso->nextFileSector += numsectors+1;
	dir->name_len = path_size - (newDirName - path) + 2;
	dir->length = sizeof(struct ISO_Directory_Entry) + dir->name_len;
	*((uint16_t *) &dir->sector.le) = newTableSector & 0xFFFF;
	*((uint16_t *) &dir->sector.be) = (newTableSector & 0xFFFF0000) >> 16;
	*((uint16_t *) &dir->size.le) = realsize & 0xFFFF;
	*((uint16_t *) &dir->size.be) = (realsize & 0xFFFF0000) >> 16;
	print_console(tasks[task_running_idx].console, "Creating File %s\n - sector %d\n - drive %c\n - %d sectors\n", newDirName, newTableSector, iso->drive->identity, numsectors);
	memcpy(dir->name, newDirName, dir->name_len-2);
	dir->name[dir->name_len - 2] = ';';
	dir->name[dir->name_len - 1] = '1';
	ISO_write_sector(iso->drive, iso->buf, parentSector);
	for(int i = 0; i < numsectors; i++){
		char *buf = (char *) ISO_read_sector(iso->drive, iso->buf, newTableSector+i);
		memset(buf, 0, 2048);
		ISO_write_sector(iso->drive, iso->buf, newTableSector+i);
	}
	return 0;
}

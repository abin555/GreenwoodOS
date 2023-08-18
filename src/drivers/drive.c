#include "drive.h"

int drive_count;
struct DRIVE **drives;
char *drive_sector_buf;

void init_drive_system(){
	print_serial("[DRIVE SYS] INIT\n");
	drives = malloc(sizeof(struct DRIVE **) * DRIVE_NUM);
	drive_sector_buf = malloc(512);
	drive_count = 0;
}

struct DRIVE *drive_add(int type, void *driver){
	struct DRIVE *new_drive = malloc(sizeof(struct DRIVE));
	drives[drive_count] = new_drive;
	new_drive->type = type;
	new_drive->identity = 'A' + drive_count;
	new_drive->locked = 0;
	new_drive->ready = 1;
	switch(new_drive->type){
		case Drive_AHCI:
			new_drive->driver.ahci = (struct HBA_PORT *) driver;
			print_serial("[DRIVE SYS] Added new AHCI drive %c:\n", new_drive->identity);
			break;
	}
	
	drive_count++;
	return new_drive;
}


int drive_read(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors){
	if(drive == NULL || buf == NULL)
		return -1;
	
	if(drive->locked) print_serial("[DRIVE SYS] Drive %c is locked\n", drive->identity);
	while(drive->locked || !drive->ready);

	drive->locked = 1;

	int err = -1;

	switch(drive->type){
		case Drive_AHCI:
			//print_serial("[DRIVE SYS] Reading Drive %c at sector %d for %d sectors\n", drive->identity, sector, count_sectors);
			err = AHCI_read((volatile HBA_PORT *) drive->driver.ahci, sector, 0, count_sectors, (uint16_t *) buf);
			while(((volatile HBA_PORT *) drive->driver.ahci)->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)){}
		break;
	}



	drive->locked = 0;
	return err;
}

struct DRIVE *drive_get(char identity){
	struct DRIVE *drive;
	for(int i = 0; i < drive_count; i++){
		drive = drives[i];
		if(drive->identity == identity) return drive;
	}
	return NULL;
}

void drive_get_format(struct DRIVE *drive){
	print_serial("[DRIVE SYS] Checking Format\n");
	if(FAT_check_format(drive)){
		drive->format = FAT32;
		return;
	}
	else if(ISO9660_check_format(drive)){
		drive->format = ISO9660;
		return;
	}

	drive->format = RAW;
	return;
}

void drive_enumerate(){
	print_serial("[DRIVE SYS] Enumerating Drives\n");
	for(int i = 0; i < drive_count; i++){
		drive_get_format(drives[i]);
	}
}

struct FILE files[NUM_FILES] = {0};

void expandPath(char *dest, int dest_size, struct DIRECTORY *dir, char *path){
	memset(dest, 0, dest_size);
	int idx = 0;
	int path_idx = 0;
	if(path[0] == '/'){
		while(path[idx+1] != 0){
			dest[idx] = path[idx+1];
			idx++;
		}
		return;
	}
	while(dir->path[idx] != 0){
		dest[idx] = dir->path[idx];
		idx++;
	}
	while(path[path_idx] != 0){
		dest[idx+path_idx] = path[path_idx];
		path_idx++;
	}
}

struct FILE *fopen_rel(struct DIRECTORY *dir, char *path){
	char big_path[200];
	memset(big_path, 0, sizeof(big_path));
	expandPath(big_path, sizeof(big_path), dir, path);
	print_serial("[Drive] Opening %s\n", big_path);
	return fopen(big_path);
}

struct FILE *fopen(char *path){
	struct FILE *file = NULL;
	for(int i = 0; i < NUM_FILES; i++){
		if(files[i].info.drive == NULL){
			file = &files[i];
			break;
		}
	}
	if(file == NULL) return NULL;
	if(path == NULL) return NULL;
	if(path[0] == '\0') return NULL;
	
	char drive_letter = path[0];
	path+=2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL) return file;
	if(drive->format == ISO9660){
		file->info = ISO9660_GetFile(drive->format_info.ISO, path);
		if(file->info.drive == NULL || file->info.sector == 0 || file->info.size == 0) return NULL;
		file->head = 0;
		return file;
	}

	return NULL;
}

void fclose(struct FILE *file){
	//memset(file, 0, sizeof(struct FILE));
	file->info.drive = NULL;
}

char fgetc(struct FILE *file){
	char *buf;
	if(file->info.drive->format == ISO9660){
		buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector);
		return buf[file->head++];
	}
	return 0;
}

int fsize(struct FILE *file){
	return file->info.size;
}

int fcopy(struct FILE *file, char *buf, int buf_size){
	if(file->info.drive->format == ISO9660){
		char *iso_buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector);
		if(iso_buf == NULL) return 1;
		int idx = 0;
		int sector_offset = 0;
		while(file->head < 512*4 && idx < buf_size){
			buf[idx] = iso_buf[file->head];
			file->head++;
			idx++;
			if(file->head == 512*4){
				sector_offset++;
				print_serial("[DRIVE] Sector Offset %d\n", sector_offset);
				iso_buf = (char *) ISO_read_sector(file->info.drive, file->info.drive->format_info.ISO->buf, file->info.sector+sector_offset);
				file->head = 0;
			}
		}

		return 0;
	}
	return 0;
}

int fexists(char *path){
	char drive_letter = path[0];
	path+=2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive == NULL) return 0;
	if(drive->format == ISO9660){
		return ISO9660_checkExists(drive->format_info.ISO, path);
	}
	return 0;
}

int changeDirectory(struct DIRECTORY *dir, char *path){
	/*
	Manipulates the task currentDirectory string.
	Everything is appended to the string except for:
	./ is ignored, and removed.
	../ removed until the previous / and continues
	- Note, ensure that ../ to idx 0 is covered.
	*/
	int path_size = 0;
	while(path[path_size] != 0){
		path_size++;
	}
	if(path[0] == '/'){//Absolute Directory
		if(fexists(path+1)){
			memset(dir->path, 0 , sizeof(dir->path));
			memcpy(dir->path, path+1, path_size-1);
			if(dir->path[path_size-2] != '/') dir->path[path_size-1] = '/';
			print_serial("Path is now %s\n", dir->path);
			return 0;
		}
		else{
			return 1;
		}
	}
	else{//Relative Directory
	}
	return 1;
}

/*
void listFiles(struct CONSOLE *console, char *path){
	char drive_letter = path[0];
	path+=2;
	struct DRIVE *drive = drive_get(drive_letter);
	if(drive->format == ISO9660){
		ISO9660_printFileList(console, drive->format_info.ISO, path);
	}
}

*/
#include <sys/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <sys/kernfns.h>
#include "structures.h"

struct DRIVE **drives;
int *drive_count;
int (*drive_read)(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);
int (*drive_write)(struct DRIVE *drive, char *buf, uint32_t sector, uint32_t count_sectors);
void (*vfs_addFS)(struct VFS_RootInterface *interface);
void *(*kmalloc)(size_t size);
void (*print_serial)(char *fmt, ...);


uint8_t *sector_buf;
FILE *serial;

uint8_t *ISO_read_sector(struct DRIVE *drive, char *buf, int sector){
	if(sector < 0) return NULL;
	drive_read(drive, (char *) buf, sector*4, 4);
	return (uint8_t *) buf;
}

void ISO_write_sector(struct DRIVE *drive, char *buf, int sector){
	drive_write(drive, (char *) buf, sector*4, 4);
}

void ISO9660_printTree_Recur(struct ISO9660 *iso, uint32_t directory_sector, int layer){
	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, directory_sector);
	char tempbuf[20];
	while(dir->length != 0){
		if(dir->name_len == 1 && (dir->name[0] == 0 || dir->name[0] == 1)){
			goto skip;
		}
		for(int i = 0; i < layer; i++){
			fprintf(serial, " ");
		}
		memset(tempbuf, 0, sizeof(tempbuf));
		memcpy(tempbuf, dir->name, dir->name_len);
		fprintf(serial, "%s @ %d\n", tempbuf, directory_sector);
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
	fprintf(serial, "ISO9660 File Tree for Drive %c\n", iso->drive->identity);
	fprintf(serial, "%c:\n", iso->drive->identity);
	ISO9660_printTree_Recur(iso, iso->root_directory_sector, 1);
}

void ISO9660_read_volume(struct ISO9660 *iso){
	fprintf(serial, "[ISO] Loading ISO9660 File Table from Drive %c\n", iso->drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) ISO_read_sector(iso->drive, iso->buf, 0x10);
	iso->root_directory_sector = getLe32(primary_vol->root_dir_ent.sector);
    fprintf(serial, "Root Directory Sector: 0x%x\n", iso->root_directory_sector);
	iso->nextTableSector = iso->root_directory_sector;
	//ISO9660_printTree(iso);
}

//Returns Directory Entry sector that matches Input String
uint32_t ISO9660_getDirectorySector(struct ISO9660 *iso, uint32_t dir_sector, char *folder){
	if(dir_sector == 0){
		return 0;
	}
	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, dir_sector);
	//print_serial("[ISO] Looking For %s\n", folder);
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

		//print_serial("Entry: %s\n", tempbuf);

		if(!(strcmp(tempbuf, folder))){
			return getLe32(dir->sector);
		}

		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
	return 0;
}

struct ISO9660_File iso9660_getFile(struct ISO9660 *iso, uint32_t dir_sector, char *file){
    struct ISO9660_File retFile = {0};
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
	//print_serial("[ISO] Looking For %s\n", namebuf);
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
			//print_serial("Found %s\n", tempbuf);
			goto foundFile;
		}

		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
	return retFile;
	foundFile:
	retFile.sector = getLe32(dir->sector);
	retFile.size = getLe32(dir->size);

	return retFile;
}

void *iso9660_vfs_get(void *fs, char *path, uint32_t *meta){
    print_serial("[ISOFS Driver] Looking for \"%s\"\n", path);
    if(fs == NULL || path == NULL || meta == NULL) return NULL;
    struct ISO9660 *iso = fs;
    struct DRIVE *drive = iso->drive;
    struct ISO9660_File *retfile = kmalloc(sizeof(struct ISO9660_File));
    char work_buf[100];
	memset(work_buf, 0, 100);
    int idx = 0;
	int work_idx = 0;

    int isFile = 0;
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
				print_serial("This is a file %s\n", work_buf);
				goto OpenFile;
			}
		}
		idx++;
		work_idx = 0;
		//print_serial("[ISO] Opening Folder %s\n", work_buf);
		dirSector = ISO9660_getDirectorySector(iso, dirSector, work_buf);
	}
	OpenFile:
	print_serial("[ISO] Looking for File %s\n", work_buf);
	*retfile = iso9660_getFile(iso, dirSector, work_buf);
	if(retfile->sector == 0){
		print_serial("[ISO] Error finding %s\n", path);
		return retfile;
	}
	print_serial("Sector %d Size %d\n", retfile->sector, retfile->size);
	return retfile;
}

int iso9660_vfs_read(void *f, void *buf, int nbytes){
    if(f == NULL || buf == NULL) return -1;
    struct VFS_File *file_idx = f;
    struct ISO9660_File *file = (struct ISO9660_File *) file_idx->inode.fs.fs;
    //print_serial("[ISO FS] Reading sector %d for %d bytes starting from %d to 0x%x\n", file->sector, nbytes, file_idx->head, buf);
    int idx = 0;
    int sector_offset = file_idx->head / (512*4);
    char *iso_buf = ISO_read_sector(file_idx->inode.interface->drive, sector_buf, file->sector+sector_offset);
    if(iso_buf == NULL) return -1;
    int walk_head = file_idx->head % (512*4);
    //print_serial("[ISO FS] Sector Offset: %d, walker head: %d\n", sector_offset, walk_head);
    while(walk_head < 512*4 && idx < nbytes){
        ((char *) buf)[idx] = iso_buf[walk_head];
        walk_head++;
        idx++;
        if(walk_head == 512*4){
            sector_offset++;
            //print_serial("[DRIVE] Sector Offset %d\n", sector_offset);
            iso_buf = (char *) ISO_read_sector(file_idx->inode.interface->drive, sector_buf, ((struct ISO9660_File *) file_idx->inode.fs.fs)->sector+sector_offset);
            walk_head = 0;
        }
    }
    file_idx->head += idx;
    return idx;
}

int iso9660_vfs_seek(void *f, int offset, int whence){
    struct VFS_File *file_idx = f;
	//print_serial("[EXT2] Seek %d to %d\n", whence, offset);
	if(whence == 0){//SEEK_SET
        file_idx->head = offset;
    }
    else if(whence == 1){//SEEK_CUR
        file_idx->head += offset;
    }
    else if(whence == 2){//SEEK_END
        file_idx->head += ((struct ISO9660_File *) file_idx->inode.fs.fs)->size + offset;
    }
    else{
        return -1;
    }
	return file_idx->head;
}

struct DirectoryListing iso9660_vfs_listDir(void *fs, char *path){
    print_serial("[ISOFS Driver] Directory Listing %s\n", path);
    struct ISO9660 *iso = fs;
	struct DirectoryListing listing = {0};
    if(fs == NULL || path == NULL) return listing;
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
        work_idx = 0;
    }
    //print_serial("Found %s at sector %d\n", path, dirSector);
    struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, dirSector);
    struct ISO_Directory_Entry *dir_walker = dir;
    memset(work_buf, 0, sizeof(work_buf));	
    listing.num_entries = 0;
    listing.directory_path_len = strlen(path);
    listing.directory_path = kmalloc(listing.directory_path_len+1);
    memcpy(listing.directory_path, path, listing.directory_path_len);
    while(dir_walker->length != 0){
        listing.num_entries++;
		dir_walker = (struct ISO_Directory_Entry *) (((uint8_t *) dir_walker) + dir_walker->length + (dir_walker->length % 2 == 0 ? 0 : 1));
	}
    listing.entries = kmalloc(sizeof(struct DirectoryEntry) * listing.num_entries);
    //print_serial("There are %d entries!\n", listing.num_entries);
    dir_walker = dir;
    int eidx = 0;
    while(dir_walker->length != 0){
        struct DirectoryEntry *entry = &listing.entries[eidx];
		if(dir_walker->name_len == 1){
			if(dir_walker->name[0] == 0){
				//print_serial(".\n");
                memcpy(entry->filename, ".", 2);
				goto finish;
			}
			else if(dir_walker->name[0] == 1){
				//print_serial("..\n");
                memcpy(entry->filename, "..", 3);
				goto finish;
			}
			goto finish;
		}
		
		if(dir_walker->name[dir_walker->name_len-2] == ';' && dir_walker->name[dir_walker->name_len-1] == '1'){
            entry->type = 0;//File
            dir_walker->name[dir_walker->name_len-2] = 0;
        }
        else{
            entry->type = 1;//Directory
        }
        entry->name_len = dir_walker->name_len;
        memset(entry->filename, 0, sizeof(entry->filename));
        if(entry->type == 0){
            entry->name_len -= 2;
        }
        memcpy(entry->filename, dir_walker->name, dir_walker->name_len);
		//print_serial("File #%d - %s (len: %d, type %d)\n", eidx, entry->filename, entry->name_len, entry->type);

		finish:
        eidx++;
		dir_walker = (struct ISO_Directory_Entry *) (((uint8_t *) dir_walker) + dir_walker->length + (dir_walker->length % 2 == 0 ? 0 : 1));
	}
    return listing;
}

void setup_iso(int drive_id){
    //task_lock(1);
    struct DRIVE *drive = drives[drive_id];
    
    printf("[ISO] Checking format on drive %c\n", drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) (ISO_read_sector(drive, (char *) sector_buf, 0x10));
    if(!(
        primary_vol->hdr.magic[0] == 'C' &&
        primary_vol->hdr.magic[1] == 'D' &&
        primary_vol->hdr.magic[2] == '0' &&
        primary_vol->hdr.magic[3] == '0' &&
        primary_vol->hdr.magic[4] == '1'
    )
    ) return;

    printf("[ISO] Drive %c Type: %d Is ISO Format: ", drive->identity, primary_vol->hdr.type);
    for(int i = 0; i < 5; i++){
        printf("%c", primary_vol->hdr.magic[i]);
    }
    printf("\n");
    struct ISO9660 *fmt = (struct ISO9660 *) kmalloc(sizeof(struct ISO9660));
    if(fmt == NULL){
        fprintf(serial, "[ISO] Error Getting Memory!\n");
        return;
    }
    drive->format = ISO9660;
    drive->format_info.ISO = fmt;
    fmt->buf = (char *) kmalloc(512*4*sizeof(uint8_t));
    fmt->drive = drive;
    ISO9660_read_volume(fmt);
    struct VFS_RootInterface *interface = kmalloc(sizeof(struct VFS_RootInterface));
    interface->drive = fmt->drive;
    interface->vfsLetter = 0;

    char fs_label[] = "ISO9660";
    char *fs_label_buf = kmalloc(sizeof(fs_label));
    memcpy(fs_label_buf, fs_label, sizeof(fs_label));
    interface->fs_label = fs_label_buf;
    
    interface->root = fmt;

    interface->fs_getLink = iso9660_vfs_get;
    interface->fs_read = iso9660_vfs_read;
    interface->fs_write = NULL;
    interface->fs_seek = iso9660_vfs_seek;
    interface->fs_creat = NULL;
    interface->fs_creatDir = NULL;
    interface->fs_listDirectory = iso9660_vfs_listDir;
    interface->fs_truncate = NULL;
    interface->fs_stat = NULL;
    vfs_addFS(interface);
    return;
}

int main(int argc, char **argv){
    serial = fopen("/-/dev/serial", "w");
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
            if(!strcmp("print_serial", def->name)){
                print_serial = def->fn;
                printf("Found print_serial!\n");
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
            default:
                printf("Drive %d (%c) is unknown format\n", i, drives[i]->identity);
                setup_iso(i);
                break;
        }
    }
    fclose(serial);
    return 0;
}
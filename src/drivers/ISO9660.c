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

void ISO9660_load_directory_table(struct ISO9660 *iso, struct ISO9660_FS_Entry *directory, uint32_t sector){
	struct ISO_Directory_Entry *dir = (struct ISO_Directory_Entry *) ISO_read_sector(iso->drive, iso->buf, sector);

	int count_children = 0;
	struct ISO_Directory_Entry *walker = dir;
	while(walker->length){
		count_children++;
		walker = (struct ISO_Directory_Entry *) (((uint8_t *) walker) + dir->length);
		//print_serial("%x\n", 0x00000000 | walker->length);
	}
	//print_serial("[ISO] Directory %s has %d children Parent is 0x%x\n", directory->name, count_children, (uint32_t) directory);
	
	directory->num_children = count_children;
	directory->children = malloc(sizeof(struct ISO9660_FS_Entry) * count_children);

	int child_idx = 0;
	while(dir->length != 0){
		struct ISO9660_FS_Entry *new_entry = &directory->children[child_idx];
		memset(new_entry, 0, sizeof(struct ISO9660_FS_Entry));
		new_entry->parent = directory;
		//print_serial("[ISO] Drive %c Directory Entry has length %d and name len %d\n", iso->drive->identity, dir->length, dir->name_len);
		//print_serial("[ISO] LBA: %x Data Length: %x Flags: %x\n", *((uint32_t *) &dir->sector.le), dir->length, dir->flags);
		if(dir->name_len == 1){
			if(dir->name[0] == 0){
				new_entry->type = Special;
				//print_serial(".  (Current Dir)\n");
				goto finish;
			}
			else if(dir->name[0] == 1){
				new_entry->type = Special;
				//print_serial(".. (Previous Dir)\n");
				goto finish;
			}
		}/*
		else{
			for(int i = 0; i < dir->name_len; i++){
				print_serial("%c", dir->name[i]);
			}
			print_serial(" - At Sector %d With Size %d\n", getLe32(dir->sector), getLe32(dir->size));
		}*/
		if(dir->flags == 2){//Create Folder
			//print_serial("[ISO] Opening Directory\n");
			memcpy(new_entry->name, dir->name, dir->name_len);
			new_entry->type = Folder;
			new_entry->sector = 0;
			new_entry->size = 0;
			new_entry->sector_count = 0;
			new_entry->parent = directory;
			ISO9660_load_directory_table(iso, new_entry, *((uint32_t *) &dir->sector.le));
			ISO_read_sector(iso->drive, iso->buf, sector);
			new_entry->type = Folder;
			new_entry->parent = directory;
			//print_serial("Parent is 0x%x\n", (uint32_t) new_entry->parent);
			child_idx++;
		}
		else{//Create File
			memcpy(new_entry->name, dir->name, dir->name_len-2);
			print_serial("[ISO] Added: %s\n", new_entry->name);
			new_entry->sector = getLe32(dir->sector);
			new_entry->size = getLe32(dir->size);
			new_entry->sector_count = (getLe32(dir->size) / (512 * 4)) + (getLe32(dir->size) % (512 * 4) == 0 ? 0 : 1);
			new_entry->parent = directory;
			new_entry->type = File;
			child_idx++;
		}
		//print_serial("Added Type %d\n", directory->children[child_idx-1].type);
		//directory->children[child_idx] = new_entry;
		directory->num_children = child_idx;
		new_entry->parent = directory;
		finish:
		dir = (struct ISO_Directory_Entry *) (((uint8_t *) dir) + dir->length + (dir->length % 2 == 0 ? 0 : 1));
	}
	//print_serial("[ISO] Exiting Directory\n");
	return;
}

void ISO9660_read_volume(struct ISO9660 *iso){
	print_serial("[ISO] Loading ISO9660 File Table from Drive %c\n", iso->drive->identity);
	struct ISO_Primary_Volume_Descriptor *primary_vol = (struct ISO_Primary_Volume_Descriptor*) ISO_read_sector(iso->drive, iso->buf, 0x10);
	iso->root.name[0] = iso->drive->identity;
	iso->root.name[1] = 0;
	iso->root.type = Root;
	iso->root.sector = getLe32(primary_vol->root_dir_ent.sector);
	iso->root.size = 0;
	iso->root.sector_count = 0;
	iso->root.parent = NULL;
	ISO9660_load_directory_table(iso, &iso->root, getLe32(primary_vol->root_dir_ent.sector));
}

void ISO9660_print_tree(struct ISO9660_FS_Entry *fs){
	if(fs->type == Special || fs == NULL){
		return;
	}
	//print_serial("Trying 0x%x\n", (uint32_t) fs);
	struct ISO9660_FS_Entry *parent = fs->parent;
	while(parent != NULL && (uint32_t) parent > 0xA0000000){
		print_serial("	");
		parent = parent->parent;
	}
	print_serial("%s @ 0x%x %d", fs->name, (uint32_t) fs, fs->type);
	if(fs->type == File || fs->type == 106496){
		print_serial(" Sector: %d Size: %d Sectors: %d", fs->sector, fs->size, fs->sector_count);
	}
	print_serial("\n");
	if(!(fs->type == Root || (fs->type == Folder || fs->sector == 0))) return;
	for(int i = 0; i < fs->num_children; i++){
		ISO9660_print_tree(&fs->children[i]);
	}
	
}

int strcmp(const char *s1, const char *s2){
	while(*s1 && (*s1 == *s2)){
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char*)s2;
}

/*
struct File ISO9660_OpenFile(char *path){
	char work_buf[20];
	memset(work_buf, 0, 20);
	struct Drive *drive;
	int idx = 0;
	int work_idx = 0;
	//Determine which drive.
	while(path[idx] != '\0' && path[idx] != '/'){
		work_buf[work_idx] = path[idx];
	}
	work_idx = 0;
	for(int i = 0; i < drive_count, i++){
		if(work_buf[1] == 0 && work_buf[0] == drives[i]->identity){
			drive = drives[i];
			break;
		}
	}

	bool foundFile = 0;
	struct ISO9660_FS_Entry *walker = &drive->format_info.ISO->root;
	while(!foundFile){
		memset(work_buf, 0, 20);
		while(path[idx] != '\0' && path[idx] != '/'){
			work_buf[work_idx] = path[idx];
		}
		work_idx = 0;

		for(int i = 0; i < walker->num_children; i++){
			if(!strcmp(work_buf, walker->children[i].name)){
				walker = &walker->children[i];
				break;
			}
		}
		continue;
		finish_dirty:
		return NULL;
	}
}
*/
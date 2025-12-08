#include <sys/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <stdlib.h>

#include <gwos/vfs.h>

#include "buffs.h"
#include "hooks.h"

struct BUFFS_llist *buffs_llist_getNth(struct BUFFS_llist *head, int n){
    if(head == NULL) return NULL;
    print_serial("[BUFFS] LLIST, getting #%d\n", n);
    for(;n > 0 && head->next != NULL; n--){
        head = head->next;
    }
    return head;
}

int buffs_llist_insert(struct BUFFS_llist *head, struct BUFFS_Inode *entry){
    if(head == NULL || entry == NULL){
        print_serial("[BUFFS] LLIST insert fail, head = 0x%x, entry = 0x%x\n", head, entry);
        return 1;
    }
    if(head->entry == NULL){//Start of linked list...
        head->entry = entry;
        head->next = NULL;
        print_serial("[BUFFS] Starting dir list!\n");
    }
    else{
        struct BUFFS_llist *new = malloc(sizeof(struct BUFFS_llist));
        new->entry = entry;
        new->next = head->next;
        head->next = new;
        print_serial("[BUFFS] LLIST, added entry\n");
    }
    return 0;
}

struct BUFFS_llist *buffs_llist_getLast(struct BUFFS_llist *head){
    if(head == NULL) return NULL;
    if(head->next == NULL) return head;
    return buffs_llist_getLast(head->next);
}

struct BUFFS_Inode *buffs_createDir(char *name){
    if(name == NULL) return NULL;
    struct BUFFS_Inode *inode = malloc(sizeof(struct BUFFS_Inode));
    inode->name = strdup(name);
    inode->creator_pid = get_task_id();
    inode->type = BUFFS_Dir;

    inode->dir = malloc(sizeof(struct BUFFS_directory));
    inode->dir->head = malloc(sizeof(struct BUFFS_llist));
    inode->dir->head->entry = NULL;
    inode->dir->head->next = NULL;
    inode->dir->num_children = 0;
    inode->parent = NULL;
    return inode;
}
struct BUFFS_Inode *buffs_createFile(char *name, size_t size){
    if(name == NULL) return NULL;
    struct BUFFS_Inode *inode = malloc(sizeof(struct BUFFS_Inode));
    inode->name = strdup(name);
    inode->creator_pid = get_task_id();
    inode->type = BUFFS_File;

    inode->file = malloc(sizeof(struct BUFFS_file));
    inode->file->size = size;
    inode->file->num_pages = size == 0 ? 0 : size / 0x400000 + 1; 
    if(inode->file->num_pages != 0){
        inode->file->page_base = (void *) MEM_reserveRegionBlock(
            MEM_findRegionIdx(inode->file->size),
            inode->file->size,
            0,
            2
        );
    }

    inode->parent = NULL;
    return inode;
}

int buffs_addInodeToDirInode(struct BUFFS_Inode *dir, struct BUFFS_Inode *child){
    if(dir == NULL || child == NULL) return 1;
    print_serial("[BUFFS] Adding \"%s\" to \"%s\"\n", child->name, dir->name);
    if(dir->type != BUFFS_Dir){
        print_serial("[BUFFS] Err, dir is wrong type\n");
        return 1;
    }
    if(dir->dir == NULL){
        print_serial("[BUFFS] Err, dir ref is invalid\n");
        return 1;
    }
    buffs_llist_insert(buffs_llist_getLast(dir->dir->head), child);
    dir->dir->num_children++;
    return 0;
}

void *buffs_getLink(void *fs, char *path, uint32_t *meta){
    if(fs == NULL || path == NULL) return NULL;
    struct BUFFS_Inode *root = fs;
    if(root == NULL) return NULL;
    if(path == NULL) return root;
    print_serial("[BUFFS] Looking for \"%s\" from \"%s\"\n", path, root->name);
    if(!strcmp(path, ".") || !strcmp(path, "")) return root;
    if(!strcmp(path, "..")) return root->parent;

    if(path[0] == 0){
        return root;
    }

    if(!strcmp(root->name, path)){
        //print_serial("[SYSFS] Found %s\n", root->name);
        return root;
    }

    int part_len = 0;
    for(int i = 0; path[i] != '\0' && path[i] != '/'; i++){
        part_len++;
    }
    char buf[100];
    memset(buf, 0, 100);
    memcpy(buf, path, part_len);

    if(root->type == BUFFS_Dir){
        for(int i = 0; i < root->dir->num_children; i++){
            struct BUFFS_llist *entry = buffs_llist_getNth(root->dir->head, i);
            struct BUFFS_Inode *inode = entry->entry;
            if(!strcmp(buf, inode->name)){
                if(inode->type == BUFFS_Dir){
                    struct BUFFS_Inode *candidate = buffs_getLink(inode, path+part_len+1, NULL);
                    if(candidate != NULL) return candidate;
                }
                else{
                    return inode;
                }
            }
        }
    }
    return NULL;
}

int buffs_read(void *f, void *buf, int nbytes){
    struct VFS_File *file = f;
    if(file == NULL || buf == NULL || nbytes == 0) return 0;
    struct BUFFS_Inode *buffs = file->inode.fs.fs;
    print_serial("[BUFFS] Read to 0x%x of %d bytes, head is at %d\n", buf, nbytes, file->head);
    if(buffs->type != BUFFS_File){
        return 0;
    }
    int i = 0;
    for(; i < nbytes && file->head < buffs->file->size; i++){
        ((char *)buf)[i] = ((char *) buffs->file->page_base)[file->head++];
    }
    return i;
}

int buffs_write(void *f, void *buf, int nbytes){
    struct VFS_File *file = f;
    if(file == NULL || buf == NULL || nbytes == 0) return 0;
    struct BUFFS_Inode *buffs = file->inode.fs.fs;
    print_serial("[BUFFS] Write from 0x%x of %d bytes, head is at %d\n", buf, nbytes, file->head);
    if(buffs->type != BUFFS_File){
        return 0;
    }
    int i = 0;
    for(; i < nbytes && file->head < buffs->file->size; i++){
        ((char *) buffs->file->page_base)[file->head++] = ((char *)buf)[i];
    }
    return i;
}

int buffs_seek(void *f, int offset, int whence){
    struct VFS_File *file_idx = f;
	//print_serial("[EXT2] Seek %d to %d\n", whence, offset);
	if(whence == 0){//SEEK_SET
        file_idx->head = offset;
    }
    else if(whence == 1){//SEEK_CUR
        file_idx->head += offset;
    }
    else if(whence == 2){//SEEK_END
        file_idx->head += ((struct BUFFS_Inode *) file_idx->inode.fs.fs)->file->size + offset;
    }
    else{
        return -1;
    }
	return file_idx->head;
}

int buffs_creat(void *fs, char *path, unsigned int size){
    struct BUFFS_Inode *buffs = fs;
    if(buffs == NULL || path == NULL) return -1;
    print_serial("[BUFFS] Creating File %s\n", path);
    if(buffs_getLink(buffs, path, NULL) != NULL){
        print_serial("[BUFFS] File %s already exists\n", path);
        return -1;
    }

    uint32_t path_length = 0;
	for(int i = 0; path[i] != '\0' && i < 255; i++){
		path_length++;
	}

	int dir_name_idx = 0;
	for(uint32_t i = 0; i < path_length; i++){
		if(path[i] == '/' && path[i+1] != '\0') dir_name_idx = i+1;
	}

	char parent_path[50];
	memset(parent_path, '\0', sizeof(parent_path));
	for(int i = 0; i < dir_name_idx && i < (int) sizeof(parent_path); i++){
		parent_path[i] = path[i];
	}

    print_serial("[BUFFS] Making File named %s from parent %s\n", path+dir_name_idx, parent_path);
    struct BUFFS_Inode *parent_inode = buffs_getLink(buffs, parent_path, NULL);
    if(parent_inode == NULL){
        print_serial("[BUFFS] Error, parent %s does not exist\n", parent_path);
        return -1;
    }
    if(parent_inode->type != BUFFS_Dir){
        print_serial("[BUFFS] Error, parent %s is not a directory\n", parent_path);
        return -1;
    }
    struct BUFFS_Inode *new_file = buffs_createFile(path+dir_name_idx, size);
    buffs_addInodeToDirInode(parent_inode, new_file);
    return 0;
}

int buffs_creatDir(void *fs, char *path){
    struct BUFFS_Inode *buffs = fs;
    if(buffs == NULL || path == NULL) return -1;
    print_serial("[BUFFS] Creating File %s\n", path);
    if(buffs_getLink(buffs, path, NULL) != NULL){
        print_serial("[BUFFS] File %s already exists\n", path);
        return -1;
    }

    uint32_t path_length = 0;
	for(int i = 0; path[i] != '\0' && i < 255; i++){
		path_length++;
	}

	int dir_name_idx = 0;
	for(uint32_t i = 0; i < path_length; i++){
		if(path[i] == '/' && path[i+1] != '\0') dir_name_idx = i+1;
	}

	char parent_path[50];
	memset(parent_path, '\0', sizeof(parent_path));
	for(int i = 0; i < dir_name_idx && i < (int) sizeof(parent_path); i++){
		parent_path[i] = path[i];
	}

    print_serial("[BUFFS] Making File named %s from parent %s\n", path+dir_name_idx, parent_path);
    struct BUFFS_Inode *parent_inode = buffs_getLink(buffs, parent_path, NULL);
    if(parent_inode == NULL){
        print_serial("[BUFFS] Error, parent %s does not exist\n", parent_path);
        return -1;
    }
    if(parent_inode->type != BUFFS_Dir){
        print_serial("[BUFFS] Error, parent %s is not a directory\n", parent_path);
        return -1;
    }
    struct BUFFS_Inode *new_file = buffs_createDir(path+dir_name_idx);
    buffs_addInodeToDirInode(parent_inode, new_file);
    return 0;
}

struct DirectoryListing buffs_advListDirectory(void *fs, char *path){
    print_serial("[BUFFS] Listing from \"%s\"\n", path);
    struct BUFFS_Inode *buffs = fs;
    struct DirectoryListing listing = {0};
    if(buffs == NULL || path == NULL) return listing;
    struct BUFFS_Inode *target = buffs_getLink(buffs, path, NULL);
    print_serial("[BUFFS] Target is \"%s\" type - %d\n", target->name, target->type);
    if(target == NULL) return listing;
    
    if(target->type != BUFFS_Dir) return listing;

    listing.directory_path_len = strlen(path);
	listing.directory_path = strdup(path);
	listing.num_entries = target->dir->num_children+2;
	listing.entries = malloc(sizeof(struct DirectoryEntry) * listing.num_entries);
	memset(listing.entries, 0, sizeof(struct DirectoryEntry) * listing.num_entries);

    const char *dotfiles[2] = {
        ".",
        ".."
    };
    for(int i = 0; i < 2; i++){
        memset(listing.entries[i].filename, 0, 50);
        memcpy(listing.entries[i].filename, (void *) dotfiles[i], sizeof(dotfiles[i])); 
        listing.entries[i].name_len = sizeof(dotfiles[i]);       
        listing.entries[i].type = ENTRY_DIRECTORY;
    }

    for(int i = 0; i < target->dir->num_children; i++){
        memset(listing.entries[i+2].filename, 0, 50);
        if(target->dir->head == NULL) continue;

        struct BUFFS_llist *entry = buffs_llist_getNth(target->dir->head, i);
        size_t name_len = strlen(entry->entry->name);
        memcpy(
            listing.entries[i+2].filename,
            entry->entry->name,
            name_len > 49 ? 49 : name_len
        );
        listing.entries[i+2].name_len = name_len > 49 ? 49 : name_len;
        if(entry->entry->type == BUFFS_Dir){
            listing.entries[i+2].type = ENTRY_DIRECTORY;
        }
        else{
            listing.entries[i+2].type = ENTRY_FILE;
        }        
    }

    return listing;
}

int buffs_truncate(void *f, unsigned int len){
    struct VFS_File *file = f;
    if(file == NULL){
        print_serial("[BUFFS] truncate error, file is null\n");
        return -1;
    }
    print_serial("[BUFFS] Resizing to %d bytes\n", len);
    struct BUFFS_Inode *root = file->inode.interface->root;
    struct BUFFS_Inode *inode = file->inode.fs.fs;
    print_serial("[BUFFS] resizing \"%s\"\n", inode->name);
    if(inode->type != BUFFS_File){
        print_serial("[BUFFS] Error, not a file\n");
        return -1;
    }
    size_t new_page_count = len / 0x400000;
    if(new_page_count == 0) new_page_count = 1;
    struct BUFFS_file *bfs_file = inode->file;
    if(new_page_count == bfs_file->num_pages){//Number of reserved pages is the same, only update structure.
        bfs_file->size = len;
        print_serial("[BUFFS] Truncate did not require page reallocation\n");
    }
    else{
        void *new_base = (void *) MEM_reserveRegionBlock(
            MEM_findRegionIdx(len),
            len,
            0,
            2
        );
        memcpy(new_base, bfs_file->page_base, len);
        MEM_freeRegionBlock((uint32_t) bfs_file->page_base, bfs_file->size);
        bfs_file->page_base = new_base;
        bfs_file->num_pages = new_page_count;
        bfs_file->size = len;
        print_serial("[BUFFS] Truncate necessitated page reallocation\n");
    }
    return 0;
}
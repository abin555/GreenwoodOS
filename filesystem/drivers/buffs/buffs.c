#include <sys/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/task.h>

#include <gwos/vfs.h>

#include "buffs.h"
#include "hooks.h"

struct BUFFS_llist *buffs_llist_getNth(struct BUFFS_llist *head, int n){
    if(head == NULL) return NULL;
    for(;n > 0 && head->next != NULL; n--){
        head = head->next;
    }
    return head;
}

struct BUFFS_Inode *buffs_createDir(char *name){
    if(name == NULL) return NULL;
    struct BUFFS_Inode *inode = malloc(sizeof(struct BUFFS_Inode));
    inode->name = strdup(name);
    inode->creator_pid = get_task_id();

    inode->dir = malloc(sizeof(struct BUFFS_directory));
    inode->dir->head = NULL;
    inode->dir->num_children = 0;
    inode->parent = NULL;
    return inode;
}
struct BUFFS_Inode *buffs_createFile(char *name, size_t size){

}

void *buffs_getLink(void *fs, char *path, uint32_t *meta){
    if(fs == NULL || path == NULL) return NULL;
    struct BUFFS_Inode *root = fs;
    if(root == NULL) return NULL;
    if(path == NULL) return root;
    if(!strcmp(path, ".")) return root;
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
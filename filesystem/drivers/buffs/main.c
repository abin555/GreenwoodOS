#include <sys/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <sys/kernfns.h>
#include <sys/sysfs.h>
#include <stdlib.h>

#include <gwos/vfs.h>

#include "buffs.h"
#include "hooks.h"

#define BUFFS_VFS_LETTER '+'

int main(int argc, char **argv){
    if(hooks_load()){
        printf("Error getting kernel hooks!\n");
        return 1;
    }


    int buffs_sysfs_fd = open("/-/buffs", O_READ);
    if(buffs_sysfs_fd != -1){
        printf("Error, buffs already loaded!\n");
        close(buffs_sysfs_fd);
        return 1;
    }
    
    FILE *metaFile = fopen("/-/fsCTRL", "r");
    if(metaFile == NULL){
        printf("Cannot open file!\n");
        return 1;
    }
    struct SysFS_Meta meta;
    fread(&meta, sizeof(meta), 1, metaFile);
    fclose(metaFile);

    void *buffs_sysfs_file = meta.mkcdev("buffs", NULL);
    meta.addChild(meta.root, buffs_sysfs_file);

    struct BUFFS_Inode *root = buffs_createDir("/");    
    root->parent = root;

    struct VFS_RootInterface *buffs_root = kmalloc(sizeof(struct VFS_RootInterface));
    *buffs_root = (struct VFS_RootInterface) {
        NULL,
        BUFFS_VFS_LETTER,
        "BUFFS",
        root,
        //Interfaces
        buffs_getLink,
        buffs_read,
        buffs_write,
        buffs_seek,
        buffs_creat,
        buffs_creatDir,
        buffs_advListDirectory,
        buffs_truncate,
        NULL
    };
    vfs_addFS(buffs_root);

    printf("BUFFS added successfully\n");
    printf("Force Exiting to keep local memory alive.\n");
    
    exit(0);

    return 0;
}
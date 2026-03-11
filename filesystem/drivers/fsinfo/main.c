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

#include "hooks.h"

#define REGION_SIZE 0x40000

int print_fs_infO(char *buf, int offset, int max, struct VFS_Inode *inode){
    if(inode == NULL || inode->isRoot == 0){
        return offset + snprintf(buf + offset, max - offset, "INVALID VFS ENTRY\n");
    }
    struct VFS_RootInterface *interface = inode->interface;
    printf("/%c - %s\n", interface->vfsLetter, interface->fs_label);
    return offset + snprintf(buf + offset, max - offset,
        "/%c - %s\n",
        interface->drive == NULL ? interface->vfsLetter : interface->drive->identity,
        interface->fs_label
    );
}

int main(int argc, char **argv){
    if(hooks_load()){
        printf("Error getting kernel hooks!\n");
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

    void *file_buf = (void *) MEM_reserveRegionBlock(MEM_findRegionIdx(REGION_SIZE), REGION_SIZE, 0, 2);

    int vfs_numRoots = vfs_meta_countRoot();
    int idx = 0;
    for(int i = 0; i < vfs_numRoots; i++){
        struct VFS_Inode *root;
        vfs_meta_getRoot(i, &root);
        idx = print_fs_infO(file_buf, idx, REGION_SIZE, root);
    }

    void *dev_folder = meta.find(meta.root, "dev\0", NULL);
    void *fs_info_dev = meta.createCdev(
        file_buf, idx+1, CDEV_READ
    );
    void *fs_info_file = meta.mkcdev("fsinfo", fs_info_dev);
    meta.addChild(meta.root, fs_info_file);
    return 0;
}
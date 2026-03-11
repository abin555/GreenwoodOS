#include <stdio.h>
#include <stdlib.h>
#include <sys/kernfns.h>
#include <gwos/kernel_fn.h>
#include <string.h>

#include "hooks.h"

int (*vfs_meta_countRoot)();
int (*vfs_meta_getRoot)(int idx, struct VFS_Inode **dst_inode_ptr);
int (*MEM_findRegionIdx)(uint32_t size);
uint32_t (*MEM_reserveRegionBlock)(int idx, uint32_t size, uint32_t virtual_base, int type);
void (*MEM_freeRegionBlock)(uint32_t virtual, uint32_t size);

int hooks_load(){
    FILE *kernel_fn_file = fopen("/-/dev/kernel", "r");
    if(kernel_fn_file != NULL){
        int num_fns;
        fread(&num_fns, sizeof(num_fns), 1, kernel_fn_file);
        struct kernel_fn_def *def;
        for(int i = 0; i < num_fns; i++){
            fread(&def, sizeof(def), 1, kernel_fn_file);
            printf("#%d: 0x%x - \"%s\"\n", i, def->fn, def->name);
            if(!strcmp("vfs_meta_countRoot", def->name)){
                vfs_meta_countRoot = def->fn;
            }
            if(!strcmp("vfs_meta_getRoot", def->name)){
                vfs_meta_getRoot = def->fn;
            }
            if(!strcmp("MEM_findRegionIdx", def->name)){
                MEM_findRegionIdx = def->fn;
            }
            if(!strcmp("MEM_reserveRegionBlock", def->name)){
                MEM_reserveRegionBlock = def->fn;
            }
            if(!strcmp("MEM_freeRegionBlock", def->name)){
                MEM_freeRegionBlock = def->fn;
            }
        }
    }
    fclose(kernel_fn_file);
    if(
        vfs_meta_countRoot == NULL ||
        vfs_meta_getRoot == NULL
    ){
        printf("Critical Error, undefined symbols!\n");
        return 1;
    }
    return 0;
}
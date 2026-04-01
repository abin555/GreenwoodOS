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
int (*select_program)(int slot);
int (*task_getCurrentID)();
int (*task_setRunningID)(int id);
int (*task_get_slot)(int task_idx);
int (*task_getCurrentPID)();
int (*taskID_fromPID)(int pid);
void (*buf_putChar)(uint32_t *buf, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void (*buf_w_putChar)(uint32_t *buf, uint32_t w, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void (*window_draw_cursor)(int x, int y);

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
            if(!strcmp("select_program", def->name)){
                select_program = def->fn;
            }
            if(!strcmp("task_getCurrentID", def->name)){
                task_getCurrentID = def->fn;
            }
            if(!strcmp("task_setRunningID", def->name)){
                task_setRunningID = def->fn;
            }
            if(!strcmp("task_get_slot", def->name)){
                task_get_slot = def->fn;
            }
            if(!strcmp("task_getCurrentPID", def->name)){
                task_getCurrentPID = def->fn;
            }
            if(!strcmp("taskID_fromPID", def->name)){
                taskID_fromPID = def->fn;
            }
            if(!strcmp("buf_putChar", def->name)){
                buf_putChar = def->fn;
            }
            if(!strcmp("buf_w_putChar", def->name)){
                buf_w_putChar = def->fn;
            }
            if(!strcmp("window_draw_cursor", def->name)){
                window_draw_cursor = def->fn;
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
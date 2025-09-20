#include <stdio.h>
#include <stdlib.h>
#include <sys/kernfns.h>
#include <string.h>

#include "hooks.h"

void (*vfs_addFS)(struct VFS_RootInterface *interface);
void *(*kmalloc)(size_t size);
void (*print_serial)(char *fmt, ...);
int (*get_task_id)();

int hooks_load(){
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
            if(!strcmp("kmalloc", def->name)){
                kmalloc = def->fn;
                printf("Found kmalloc!\n");
            }
            if(!strcmp("print_serial", def->name)){
                print_serial = def->fn;
                printf("Found print_serial!\n");
            }
            if(!strcmp("get_task_id", def->name)){
                get_task_id = def->fn;
                printf("Get Task ID Found!\n");
            }
        }
    }
    fclose(kernel_fn_file);
    if(
        vfs_addFS == NULL ||
        print_serial == NULL ||
        kmalloc == NULL
    ){
        printf("Critical Error, undefined symbols!\n");
        return 1;
    }
    return 0;
}
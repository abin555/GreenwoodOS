#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/task.h>
#include <sys/sysfs.h>

int wasRead;
int task_id;
int (*vfs_open)(char *file, int perms);
int (*vfs_write)(int fd, char *buf, int size);
void (*vfs_close)(int fd);
void (*kern_set_schedule_task)(int task_id, int sched);

struct kernel_fn_def {
    void *fn;
    int n_args;
    int name_len;
    char name[];
};

void my_read(void *file, int offset, int nbytes, int *head){
    wasRead = 1;
    int serial_fd = vfs_open("-/dev/serial", 2);
    if(serial_fd != -1){
        vfs_write(serial_fd, "[DRV] READ!\n", 13);
        vfs_close(serial_fd);
    }
    if(task_id != -1){
        kern_set_schedule_task(task_id, ALWAYS);
    }
}

int main(int argc, char **argv){
    task_id = -1;
    FILE *kernel_fn_file = fopen("/-/dev/kernel", "r");
    if(kernel_fn_file != NULL){
        int num_fns;
        fread(&num_fns, sizeof(num_fns), 1, kernel_fn_file);
        struct kernel_fn_def *def;
        for(int i = 0; i < num_fns; i++){
            fread(&def, sizeof(def), 1, kernel_fn_file);
            if(!strcmp("open", def->name)){
                vfs_open = def->fn;
                printf("Found open!\n");
            }
            if(!strcmp("write", def->name)){
                vfs_write = def->fn;
                printf("Found write!\n");
            }
            if(!strcmp("close", def->name)){
                vfs_close = def->fn;
                printf("Found close!\n");
            }
            if(!strcmp("set_schedule_task", def->name)){
                kern_set_schedule_task = def->fn;
            }
            if(!strcmp("get_task_id", def->name)){
                task_id = ((int (*)()) def->fn)();
            }
        }
    }
    else{
        vfs_open = NULL;
        vfs_write = NULL;
    }

    FILE *metaFile = fopen("/-/fsCTRL", "r");
    if(metaFile == NULL){
        printf("Cannot open file!\n");
        return 1;
    }
    struct SysFS_Meta meta;
    fread(&meta, sizeof(meta), 1, metaFile);
    fclose(metaFile);
    void *dir = meta.mkdir("Test");
    meta.addChild(meta.root, dir);

    char *buf = malloc(11);
    memcpy(buf, "My Buffer!", 11);

    void *cdev = meta.createCdev(
        buf,
        11,
        CDEV_READ
    );
    meta.setCallbacks(cdev,
        NULL,
        my_read,
        NULL,
        NULL
    );
    void *test = meta.mkcdev("dev", cdev);
    meta.addChild(dir, test);

    wasRead = 0;
    while(1){
        if(wasRead){
            printf("Read!\n");
            wasRead = 0;
        }
        set_schedule(NEVER);
        yield();
    }
}
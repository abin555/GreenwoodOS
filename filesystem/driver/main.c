#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/task.h>
#include <sys/sysfs.h>

int wasRead;

void my_read(void *file, int offset, int nbytes, int *head){
    wasRead = 1;
}

int main(int argc, char **argv){
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
        yield();
    }
}
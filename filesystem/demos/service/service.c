#include <stdio.h>
#include <sys/sysfs.h>
#include <sys/io.h>
#include <sys/task.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

FILE *serial;

void fib(int n){
    int i;

    // initialize first and second terms
    int t1 = 0, t2 = 1;

    // initialize the next term (3rd term)
    int nextTerm = t1 + t2;

    // print the first two terms t1 and t2
    fprintf(serial, "Fibonacci Series: %d, %d, ", t1, t2);

    // print 3rd to nth terms
    for (i = 3; i <= n; ++i) {
        fprintf(serial, "%d, ", nextTerm);
        t1 = t2;
        t2 = nextTerm;
        nextTerm = t1 + t2;
    }
    fprintf(serial, "\n");
}

#define SERVICE_FILE_SIZE 1000

char *service_file_buf;
int num_reads;

void read_notification(void *file, int offset, int nbytes, int *head){
    memset(service_file_buf, 0, SERVICE_FILE_SIZE);
    snprintf(service_file_buf, SERVICE_FILE_SIZE, "Read %d times!\n\n", ++num_reads);
}

int main(){
    printf("Hello from Service! Fib is at 0x%x\n", &fib);
    serial = fopen("/-/dev/serial", "w");
    fib(10);

    FILE *metaFile = fopen("/-/fsCTRL", "r");
    if(metaFile == NULL){
        printf("Cannot open file!\n");
        return 1;
    }
    struct SysFS_Meta meta;
    fread(&meta, sizeof(meta), 1, metaFile);
    fclose(metaFile);

    service_file_buf = malloc(SERVICE_FILE_SIZE);
    num_reads = 0;
    
    memset(service_file_buf, 0, SERVICE_FILE_SIZE);
    snprintf(service_file_buf, SERVICE_FILE_SIZE, "Read %d times!", num_reads);

    void *cdev = meta.createCdev(
        service_file_buf,
        SERVICE_FILE_SIZE,
        CDEV_READ
    );

    meta.setCallbacks(cdev,
        NULL,
        read_notification,
        NULL,
        NULL
    );
    void *test = meta.mkcdev("file", cdev);


    void *dir = meta.mkdir("Serv");
    meta.addChild(meta.root, dir);

    meta.addChild(dir, test);
    set_schedule(NEVER);
    while(1){
        
    }
}
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define bufsize 1000
char buf[bufsize];
int main(int argc, char **argv){
    if(argc != 2){
        return 1;
    }
    //freopen("/-/dev/serial", "rw+", stdout);
    FILE *f = fopen(argv[1], "r");
    if(f == NULL){
        printf("File %s does not exist\n", argv[1]);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    printf("Size %d\n", size);
    fseek(f, 0, SEEK_SET);
    int remaining = size;
    for(int i = 0; i < (size / bufsize)+1; i++){
        int read_amount = remaining > bufsize ? bufsize : remaining;
        int n = fread(buf, read_amount, 1, f);
        remaining -= read_amount;
        fwrite(buf, read_amount, 1, stdout);
    }
    fclose(f);

    return 0;
}
#include <stdio.h>
#include <sys/io.h>
#include <sys/task.h>

int main(int argc, char **argv){
    printf("SHELL >");

    char readbuf[10];
    while(1){
        int n = read(0, readbuf, 1);
        if(n > 0){
            fwrite(readbuf, 1, 1, stdout);
        }
        
        yield();
    }

    return 0;
}
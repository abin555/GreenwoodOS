#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/io.h>
#include <string.h>

int main(int argc, char **argv){
    printf("Pipe test program!\n");
    int pipedescs[2];
    char msg[] = "This is a test message!";
    int status = pipe(pipedescs);
    printf("Pipe created with status %d, writer fd: %d, reader fd: %d\n", status, pipedescs[0], pipedescs[1]);

    int nwrite = write(pipedescs[0], msg, sizeof(msg));
    printf("Wrote %d bytes into pipe!\n", nwrite);

    char readbuf[100];
    memset(readbuf, 0, sizeof(readbuf));
    int nread = read(pipedescs[1], readbuf, sizeof(readbuf));
    printf("Read %d bytes from pipe\n\"%s\"\n", nread, readbuf);

    dup2(pipedescs[0], 1);
    printf("Test print!\n");


    close(pipedescs[0]);
    close(pipedescs[1]);
    return 0;
}
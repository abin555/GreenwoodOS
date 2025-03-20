#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
    puts("Fork Test!\n");
    int pid = fork();
    if(pid != 0){
        puts("Child Process!\n");
    }
    else{
        puts("Parent Process!\n");
    }
    char buf[2] = "0\0";
    buf[0] += pid;
    puts("PID: ");
    puts(buf);
    puts("\n");
    return 0;
}
#include "libc.h"

int main(int argc, char **argv){
    dprint("Fork Test!\n");
    int pid = fork();
    if(pid != 0){
        dprint("Child Process!\n");
    }
    else{
        dprint("Parent Process!\n");
    }
    char buf[2] = "0\0";
    buf[0] += pid;
    dprint("PID: ");
    dprint(buf);
    dprint("\n");
    return 0;
}
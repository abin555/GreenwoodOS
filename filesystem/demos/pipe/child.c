#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/io.h>
#include <string.h>
#include <sys/task.h>

int main(int argc, char **argv){
    puts("This is the child process!");
    puts("This is the child process 2!");
    puts("This is the child process 3!");
    return 0;
}
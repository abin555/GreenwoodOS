#include <stdio.h>
#include <stdlib.h>

void _Assert(char *mesg){
    fprintf(stderr, mesg);
    fprintf(stderr, " -- assertion failed\n");
    exit(1);
}
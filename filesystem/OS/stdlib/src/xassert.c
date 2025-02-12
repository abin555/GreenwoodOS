#include "libc.h"

void _Assert(char *mesg){
    print(mesg);
    print(" -- assertion failed\n");
    exit(1);
}
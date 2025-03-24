#include <unistd.h>
#include <stdint.h>
#include <sys/dir.h>

int chdir(const char *path){
    if(changeDirectory((char *) path)){
        return -1;
    }
    return 0;
}
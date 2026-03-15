#include <unistd.h>
#include <string.h>
#include <sys/dir.h>

char *getcwd(char *buf, int size){
    char *dir_str = getDirectory();
    if(dir_str == NULL){
        return buf;
    }
    strncpy(buf, dir_str, size);
    return buf;
}
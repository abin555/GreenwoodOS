#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/task.h>

int main(int argc, char **argv){
    freopen("/-/dev/serial", "w", stdout);
    FILE *autostart = fopen("/A/OS/autostart.ini", "r");
    if(autostart == NULL){
        printf("[INIT] Unable to open autostart.ini file!\n");
        return 1;
    }

    fseek(autostart, 0, SEEK_END);
    int size = ftell(autostart);
    char *text = malloc(size);
    fseek(autostart, 0, SEEK_SET);
    fread(text, size, 1, autostart);
    fclose(autostart);

    puts(text);

    return 0;
}
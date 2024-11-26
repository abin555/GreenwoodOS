#include "libc.h"

char name_local[] = "Virus\0";

int main(int argc, char **argv){
    void *payload_address = requestRegionAt(0x400000, 0x400000);
    struct FILE *payload_file = fopen("./payload.exe");
    fcopy(payload_file, payload_address, fsize(payload_file));
    char *name = kmalloc(sizeof(name_local));
    for(int i = 0; i < sizeof(name_local); i++) name[i] = name_local[i];
    start_manual_task(payload_address, name);
    return 0;
}
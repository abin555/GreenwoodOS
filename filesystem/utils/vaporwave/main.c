#include "libc.h"

const char BACKGROUND[] =  "/A/Pictures/vaporwave.tga";
const char MODEL[] = "/A/Models/helios.obj";
const char MUSIC[] = "/A/Audio/Macintosh.wav";

void *alloc_base;
uint32_t alloc_size;
void *alloc_mover;

void setup_alloc(){
    alloc_size = 0x40000;
    alloc_base = requestRegion(alloc_size);
    alloc_mover = alloc_base;
}
void *alloc(int size){
    void *addr = alloc_mover;
    alloc_mover += size+1;
    for(int i = 0; i < size+1;i++){
        *(char *) addr = '\0';
    }
    return addr;
}
void clean_alloc(){
    freeRegion(alloc_base, alloc_size);
}

int main(int argc, char **argv){
    setup_alloc();

    char **bg_argv = alloc(sizeof(char *)*2);
    bg_argv[0] = NULL;
    bg_argv[1] = alloc(sizeof(BACKGROUND));
    for(int i = 0; i < sizeof(BACKGROUND); i++){
        bg_argv[1][i] = BACKGROUND[i];
    }
    dprint(bg_argv[1]);
    dprint("\n");
    exec("/A/utils/SetBG/setbg.exe", 2, bg_argv);

    char **md_argv = alloc(sizeof(char *)*2);
    md_argv[0] = NULL;
    md_argv[1] = alloc(sizeof(MODEL));
    for(int i = 0; i < sizeof(MODEL); i++){
        md_argv[1][i] = MODEL[i];
    }
    dprint(md_argv[1]);
    dprint("\n");
    exec("/A/utils/3D/3Dvp.exe", 2, md_argv);

    char **wv_argv = alloc(sizeof(char *)*2);
    wv_argv[0] = NULL;
    wv_argv[1] = alloc(sizeof(MUSIC));
    for(int i = 0; i < sizeof(MUSIC); i++){
        wv_argv[1][i] = MUSIC[i];
    }
    dprint(wv_argv[1]);
    dprint("\n");
    exec("/A/utils/Music/music.exe", 2, wv_argv);

    for(int i = 0; i < 0xFFFFFF; i++){
        i++;
        i--;
    }

}
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/task.h>

const char BACKGROUND[] =  "/A/Pictures/vaporwave.tga";
const char MODEL[] = "/A/Models/helios.obj";
const char MUSIC[] = "/A/Audio/Macintosh.wav";

int main(int argc, char **argv){
    char **bg_argv = malloc(sizeof(char *)*2);
    bg_argv[0] = NULL;
    bg_argv[1] = malloc(sizeof(BACKGROUND));
    for(int i = 0; i < sizeof(BACKGROUND); i++){
        bg_argv[1][i] = BACKGROUND[i];
    }
    puts(bg_argv[1]);
    puts("\n");
    exec("/A/utils/SetBG/setbg.elf", 2, bg_argv);

    char **md_argv = malloc(sizeof(char *)*3);
    md_argv[0] = NULL;
    md_argv[1] = malloc(sizeof(MODEL));
    for(int i = 0; i < sizeof(MODEL); i++){
        md_argv[1][i] = MODEL[i];
    }
    puts(md_argv[1]);
    puts("\n");
    exec("/A/utils/3D/3Dvp.elf", 3, md_argv);

    char **wv_argv = malloc(sizeof(char *)*2);
    wv_argv[0] = NULL;
    wv_argv[1] = malloc(sizeof(MUSIC));
    for(int i = 0; i < sizeof(MUSIC); i++){
        wv_argv[1][i] = MUSIC[i];
    }
    puts(wv_argv[1]);
    puts("\n");
    exec("/A/utils/Music/music.elf", 2, wv_argv);

    for(int i = 0; i < 0xFFFFFF; i++){
        i++;
        i--;
    }
}
#include <stdio.h>
#include <stdlib.h>

struct DesktopConfig {
    int vp_root_x;
    int vp_root_y;
    int screen_w;
    int screen_h;
};

int main(int argc, char **argv){
    if(argc != 3){
        printf("Invalid Args\nUsage: x y\n");
        return 1;
    }
    int rootx = -1 * atoi(argv[1]);
    int rooty = -1 * atoi(argv[2]);

    FILE *screenConf = fopen("/-/sys/screen", "rw");
    if(screenConf == NULL){
        printf("Error, unable to open screen conf\n");
        return 1;
    }
    struct DesktopConfig conf;
    fread(&conf, sizeof(conf), 1, screenConf);
    //printf("Changing Root - StartX: %d StartY: %d\n", conf.vp_root_x, conf.vp_root_y);
    conf.vp_root_x = rootx;
    conf.vp_root_y = rooty;
    //printf("Changing Root - NewX: %d NewY: %d\n", conf.vp_root_x, conf.vp_root_y);
    fseek(screenConf, 0, SEEK_SET);
    fwrite(&conf, sizeof(conf), 1, screenConf);
    fclose(screenConf);
    return 0;
}
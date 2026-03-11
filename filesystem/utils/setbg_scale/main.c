#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/task.h>

#define RSIZE_FILE "/+/rsize_bg.tga"

int mkResizedFile(char *filename, int w, int h){
    char wstr[10];
    int n = snprintf(wstr, sizeof(wstr), "%d\0", w);
    wstr[n] = '\0';
    char hstr[10];
    n = snprintf(hstr, sizeof(hstr), "%d\0", h);
    hstr[n] = '\0';
    char **argv = malloc(sizeof(char *) * 6);
    argv[0] = "/A/utils/rsize/rsize.elf";
    argv[1] = strdup(filename);
    argv[2] = strdup(RSIZE_FILE);
    argv[3] = strdup(wstr);
    argv[4] = strdup(hstr);
    argv[5] = NULL;
    int pid = exec(argv[0], 5, argv);
    waitpid(pid);
    printf("Icon Hopefully Resized!\n");
}

struct DesktopConfig {
    int vp_root_x;
    int vp_root_y;
    int screen_w;
    int screen_h;
};

int main(int argc, char **argv){
    if(argc != 2){
        printf("Err, please provide background file!\n");
        return 1;
    }

    int screen_fd = open("/-/sys/screen", O_READ | O_WRITE);
    struct DesktopConfig conf;
    read(screen_fd, &conf, sizeof(struct DesktopConfig));
    printf("Screen is %dx%d\n", conf.screen_w, conf.screen_h);
    close(screen_fd);

    mkResizedFile(argv[1], conf.screen_w, conf.screen_h);

    char **proc_argv = malloc(sizeof(char *) * 6);
    proc_argv[0] = "/A/utils/SetBG/setbg.elf";
    proc_argv[1] = strdup(RSIZE_FILE);
    proc_argv[3] = NULL;
    int pid = exec(proc_argv[0], 2, proc_argv);
    waitpid(pid);

    return 0;
}
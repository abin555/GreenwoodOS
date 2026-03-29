#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/window.h>
#include <sys/task.h>
#include <sys/mouse.h>
#include <string.h>

int main(int argc, char **argv){
    struct WINDOW *win = window_open("Desktop", 1);
    set_schedule(ONFOCUS);

    FILE *mouseFile = fopen("/-/sys/mouse", "r");
    struct MouseStatus mouse;

    uint32_t bufsize = win->width * win->height * sizeof(uint32_t);

    while(1){
        fseek(mouseFile, 0, SEEK_SET);
        fread(&mouse, sizeof(mouse), 1, mouseFile);

        memset(win->backbuffer, 0, bufsize);
        win->backbuffer[mouse.pos.x + win->width * mouse.pos.y] = 0xFFFFFF;
        
        window_update();
        yield();
    }

    window_close(win);
}
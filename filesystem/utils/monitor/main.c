#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/task.h>
#include <string.h>

int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    switch(event){
        case VP_MAXIMIZE:
            set_schedule(ALWAYS);
            break;
        case VP_MINIMIZE:
            set_schedule(NEVER);
            break;
        case VP_EXIT:
            set_schedule(ALWAYS);
            running = 0;
            break;
    }
}

int main(int argc, char **argv){
    if(argc != 2){
        printf("No file provided");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if(f == NULL){
        printf("Unable to open file\n");
    }
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char title_buf[100];
    int n = snprintf(
        title_buf,
        sizeof(title_buf),
        "Monitor %s",
        argv[1]
    );
    title_buf[n] = '\0';

    char *file_buf = malloc(len);
    fseek(f, 0, SEEK_SET);
    fread(file_buf, len, 1, f);
    int fwidth = 0;
    int walk_width = 0;
    int fheight = 0;
    for(int i = 0; i < len; i++){
        if(file_buf[i] == '\n'){
            fheight++;
            walk_width = 0;
        }{
            walk_width++;
            if(walk_width > fwidth){
                fwidth = walk_width;
            }
        }
    }

    struct Viewport *vp = vp_open(
        8*fwidth,
        8*fheight,
        title_buf
    );
    uint32_t bufsize = sizeof(uint32_t) * 8*fwidth * 8*fheight;
    vp_set_buffer(vp, malloc(bufsize), bufsize);
    vp_add_event_handler(vp, event_handler);
    
    running = 1;
    while(running){
        fseek(f, 0, SEEK_SET);
        fread(file_buf, len, 1, f);
        fwidth = 0;
        walk_width = 0;
        fheight = 0;
        for(int i = 0; i < len && file_buf[i] != '\0'; i++){
            if(file_buf[i] == '\n'){
                fheight++;
                walk_width = 0;
            }{
                walk_width++;
                if(walk_width > fwidth){
                    fwidth = walk_width;
                }
            }
        }

        int x = 0;
        int y = 0;

        memset(vp->backbuf, 0, vp->buf_size);

        for(int i = 0; i < len && file_buf[i] != '\0'; i++){
            if(file_buf[i] == '\n'){
                x = 0;
                y += 8;
            }
            else{
                vp_drawChar(vp, x, y, file_buf[i], 0xFFFFFF, 0x0);
                x += 8;
            }
        }

        vp_copy(vp);
        yield();
    }

    fclose(f);
    vp_close(vp);
}
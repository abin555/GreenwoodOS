#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/io.h>
#include <sys/task.h>
#include <string.h>
#include <sys/mouse.h>
#include <sys/memory.h>
#include "tbar.h"
#include "lexer.h"

struct DesktopConfig {
    int vp_root_x;
    int vp_root_y;
    int screen_w;
    int screen_h;
};

uint32_t width;
uint32_t height;
uint32_t bufsize;
uint32_t *backbuf;

void parse(struct Taskbar *taskbar, struct TokenStream *s){
    for(int i = 0; i < s->ntokens; i++){
        if(s->tokens[i].type == STR && i <= s->ntokens - 5){
            if(
                s->tokens[i + 1].type == STR && 
                s->tokens[i + 2].type == STR && 
                s->tokens[i + 3].type == STR &&
                s->tokens[i + 4].type == ENDL 
            ){
                tbar_entry_init(
                    &taskbar->entries[taskbar->numEntries++],
                    s->tokens[i + 0].str,
                    s->tokens[i + 1].str,
                    s->tokens[i + 2].str,
                    s->tokens[i + 3].str
                );
            }
        }
    }
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    printf("Event %d\n", event);
    if(event == VP_CLICK){
        set_schedule(ALWAYS);
    }
    if(event == VP_MINIMIZE){
        vp->minimized = 0;
        vp->loc.w = vp->minimized_w;
        vp->loc.h = vp->minimized_h;
    }
}

int main(int argc, char **argv){
    freopen("/-/dev/serial", "rw", stdout);

    struct Taskbar *tbar = tbar_init();
    if(argc == 2){
        //printf("Opening - %s\n", argv[1]);
        FILE *conf = fopen(argv[1], "r");
        fseek(conf, 0, SEEK_END);
        int size = ftell(conf);
        printf("Conf file is %d bytes\n", size);
        fseek(conf, 0, SEEK_SET);
        char *buf = malloc(size+1);
        memset(buf, 0, size+1);
        fread(buf, size, 1, conf);
        fclose(conf);
        printf("Read file!\n");
        struct TokenStream s = lex(buf, size);
        printf("Lexed!\n");
        printTokenStream(s);
        parse(tbar, &s);
    }

    int screen_fd = open("/-/sys/screen", O_READ | O_WRITE);
    struct DesktopConfig conf;
    read(screen_fd, &conf, sizeof(struct DesktopConfig));
    printf("[TBAR] Screen is %dx%d\n", conf.screen_w, conf.screen_h);

    struct DesktopConfig mutDesktopConfig = conf;
    mutDesktopConfig.vp_root_x = 0;
    mutDesktopConfig.vp_root_y = conf.screen_h-(TBAR_HEIGHT+10);
    lseek(screen_fd, 0, SEEK_SET);
    write(screen_fd, &mutDesktopConfig, sizeof(mutDesktopConfig));

    width = conf.screen_w;
    height = TBAR_HEIGHT;

    struct Viewport *tbar_vp = vp_open(width, height, "Taskbar");
    lseek(screen_fd, 0, SEEK_SET);
    write(screen_fd, &conf, sizeof(conf));
    close(screen_fd);    
    
    vp_set_options(tbar_vp, VP_OPT_NODECORATOR);
    bufsize = sizeof(uint32_t*) * width * height;
    backbuf = malloc(bufsize);
    vp_set_buffer(tbar_vp, backbuf, bufsize);

    

    for(int i = 0; i < tbar->numEntries; i++){
        tbar_drawIcon(i+1, tbar->entries[i].icon, backbuf, width, height);
    }
    vp_copy(tbar_vp);

    tbar_vp->click_events_enabled = 1;
    vp_add_event_handler(tbar_vp, event_handler);

    FILE *mouse = fopen("/-/sys/mouse", "r");
    struct MouseStatus mouseStatus;

    set_schedule(NEVER);
    yield();

    while(1){
        printf("[TBAR] Tick\n");

        fseek(mouse, 0, SEEK_SET);
        fread(&mouseStatus, sizeof(mouseStatus), 1, mouse);

        int rely = mouseStatus.pos.y - tbar_vp->loc.y;
        if(rely > 0){
            int idx = (mouseStatus.pos.x / TBAR_HEIGHT) - 1;
            printf("%d %d %d Clicked on %s\n", mouseStatus.pos.x, rely, idx, tbar->entries[idx].name);
            exec(tbar->entries[idx].exec_filename, 0, NULL);
        }

        set_schedule(NEVER);
        yield();
    }
    return 0;
}


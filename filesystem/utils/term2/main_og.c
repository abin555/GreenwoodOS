#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/task.h>
#include <sys/io.h>
#include <sys/vp.h>
#include <sys/dir.h>
#include <sys/console.h>
#include <sys/memory.h>
#include <sys/stat.h>
#include <exec/exec.h>
#include <string.h>

#define TERM_W 40
#define TERM_H 40

#define TERM_MAX_W 200
#define TERM_MAX_H 200

#define CHAR_SCALE 8

struct Terminal {
    int term_w;
    int term_h;
    int char_size;
    char *text_buf;
    int cursor_idx;
    int text_buf_len;

    int vp_w;
    int vp_h;
    uint32_t bufsize;
    uint32_t *buf;
    struct Viewport *vp;
};

struct Terminal *g_term;

void term_resize(struct Terminal *term, int vp_w, int vp_h);

void term_ev_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    switch(event){
        case VP_RESIZE:
            term_resize(g_term, vp->resizeLoc.w, vp->resizeLoc.h);
            vp->loc.w = g_term->vp_w;
            vp->loc.h = g_term->vp_h;
            break;
    }
}

void term_drawBars(struct Terminal *term){
    for(int x = 0; x < term->term_w; x++){
        vp_drawChar(term->vp, x*term->char_size, (term->term_h-1) * term->char_size, '-', 0x000000, 0xFFFFFF);
    }
    for(int y = 0; y < term->term_h; y++){
        vp_drawChar(term->vp, (term->term_w-1) * term->char_size, y*term->char_size, '|', 0x000000, 0xFFFFFF);
    }
    vp_copy(term->vp);
}

void term_draw(struct Terminal *term){
    memset(term->buf, 0, term->bufsize);
    int x = 0;
    int y = 0;
    uint32_t fg = 0xFFFFFF;
    uint32_t bg = 0x000000;
    for(int i = 0; i < term->text_buf_len && y < term->term_h-1; i++){
        if(term->text_buf[i] == '\n'){
            x = 0;
            y++;
            continue;
        }
        vp_drawChar(term->vp, x*term->char_size, y*term->char_size, term->text_buf[i], (i == term->cursor_idx) ? bg : fg, (i == term->cursor_idx) ? fg : bg);
        x++;
        if(x == term->term_w-1){
            x = 0;
            y++;
        }
    }
    term_drawBars(term);
}

void term_resize(struct Terminal *term, int vp_w, int vp_h){
    term->term_w = (vp_w / term->char_size) + 1;
    term->term_h = (vp_h / term->char_size) + 1;
    term->text_buf_len = term->term_w * term->term_h;

    term->vp_w = term->term_w * term->char_size;
    term->vp_h = term->term_h * term->char_size;
    printf("Requested %dx%d - Term %dx%d - Target %dx%d\n", vp_w, vp_h, term->term_w, term->term_h, term->vp_w, term->vp_h);
    memory_returnRegion(term->buf, term->bufsize);
    term->bufsize = sizeof(uint32_t) * term->vp_w * term->vp_h;
    term->buf = memory_requestRegion(sizeof(uint32_t) * term->vp_w * term->vp_h);
    vp_set_buffer(term->vp, term->buf, term->bufsize);
    term_draw(term);
    vp_copy(term->vp);
    set_schedule(ALWAYS);
}

struct Terminal *term_init(){
    struct Terminal *term = malloc(sizeof(struct Terminal));
    term->term_w = TERM_W;
    term->term_h = TERM_H;
    term->char_size = CHAR_SCALE;
    term->text_buf = malloc(sizeof(char) * TERM_MAX_W * TERM_MAX_H);
    memset(term->text_buf, 0, sizeof(term->text_buf));
    term->cursor_idx = 0;
    term->text_buf_len = term->term_w * term->term_h;

    term->vp_w = term->term_w * term->char_size;
    term->vp_h = term->term_h * term->char_size;
    term->bufsize = sizeof(uint32_t) * term->vp_w * term->vp_h;
    term->buf = memory_requestRegion(sizeof(uint32_t) * term->vp_w * term->vp_h);
    term->vp = vp_open(term->vp_w, term->vp_h, "GWOS Terminal 2");
    vp_add_event_handler(term->vp, term_ev_handler);
    vp_set_buffer(term->vp, term->buf, term->bufsize);
    vp_set_options(term->vp, VP_OPT_RESIZE);
    return term;
}

int main(int argc, char **argv){
    g_term = term_init();
    int ch_stdout_pipe[2];
    int ch_stdin_pipe[2];
    pipe(ch_stdout_pipe);
    pipe(ch_stdin_pipe);
    printf("Pipe FDs -> %d, %d\n", ch_stdout_pipe[0], ch_stdout_pipe[1]);
    printf("Pipe FDs -> %d, %d\n", ch_stdin_pipe[0], ch_stdin_pipe[1]);

    struct stat pipe_stat;
    fstat(ch_stdout_pipe[0], &pipe_stat);

    printf("Pipe stat owner: %s\n", pipe_stat.fs_ownerIden);
    printf("Pipe open stat: %d\n", pipe_stat.open_stat);

    EXEC_CTX proc_ctx = exec_createCTX();
    exec_setProgram(proc_ctx, "/A/utils/sh/sh.elf");
    exec_setARGC(proc_ctx, 0);
    exec_setFDCount(proc_ctx, 2);
    exec_setFD(proc_ctx, 0, ch_stdin_pipe[1], 0);
    exec_setFD(proc_ctx, 1, ch_stdout_pipe[0], 1);
    int child_pid = exec_simple(proc_ctx);
    
    
    char readbuf[10];
    char writebuf[10];
    int i = 0;
    while(1){
        //fstat(ch_stdout_pipe[0], &pipe_stat);
        if(g_term->vp->ascii != '\0'){
            writebuf[0] = g_term->vp->ascii;
            write(ch_stdin_pipe[0], writebuf, 1);
            g_term->vp->ascii = '\0';
        }
        int n = read(ch_stdout_pipe[1], readbuf, 1);

        if(n == 1){
            char c = readbuf[0];
            if(c == 8){
                g_term->text_buf[--g_term->cursor_idx] = '\0';
                term_draw(g_term);
            }
            else{
                g_term->text_buf[g_term->cursor_idx++] = readbuf[0];
                term_draw(g_term);
            }
        }
        else{
            yield();
        }
    }
}
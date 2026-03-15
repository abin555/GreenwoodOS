#include <sys/vp.h>
#include <sys/io.h>

struct ViewportFunctions {
    struct Viewport *(*open)(int, int, char*);
    void (*close)(struct Viewport*);
    void (*set_buffer)(struct Viewport*, uint32_t*, uint32_t);
    void (*copy)(struct Viewport*);
    void (*add_event_handler)(struct Viewport *, void (*)(struct Viewport *, VIEWPORT_EVENT_TYPE));
    void (*drawChar)(struct Viewport *, int, int, char, uint32_t, uint32_t);
    char (*getc)(struct Viewport *);
    void (*set_options)(struct Viewport*, VIEWPORT_OPTIONS);
};

struct ViewportFunctions vp_functions;
int sys_vp_initialized = 0;
void vp_init(){
    int vp_file = open("/-/sys/viewport", O_READ);
    if(vp_file != -1){
        read(vp_file, &vp_functions, sizeof(struct ViewportFunctions));
        sys_vp_initialized = 1;
    }
    close(vp_file);
}

struct Viewport *vp_open(int w, int h, char *title){
    if(!sys_vp_initialized) vp_init();
    return vp_functions.open(w, h, title);
}

void vp_close(struct Viewport *vp){
    if(!sys_vp_initialized) vp_init();
    vp_functions.close(vp);
}

void vp_set_buffer(struct Viewport *vp, uint32_t *buf, uint32_t size){
    if(!sys_vp_initialized) vp_init();
    vp_functions.set_buffer(vp, buf, size);
}

void vp_copy(struct Viewport *vp){
    if(!sys_vp_initialized) vp_init();
    vp_functions.copy(vp);
}

void vp_add_event_handler(struct Viewport *vp, void (*handler)(struct Viewport *, VIEWPORT_EVENT_TYPE)){
    if(!sys_vp_initialized) vp_init();
    vp_functions.add_event_handler(vp, handler);
}

void vp_drawChar(struct Viewport *vp, int x, int y, char c, uint32_t fg, uint32_t bg){
    if(!sys_vp_initialized) vp_init();
    vp_functions.drawChar(vp, x, y, c, fg, bg);
}

char vp_getc(struct Viewport *vp){
    if(!sys_vp_initialized) vp_init();
    vp_functions.getc(vp);
}

void vp_set_options(struct Viewport *vp, VIEWPORT_OPTIONS options){
    if(!sys_vp_initialized) vp_init();
    vp_functions.set_options(vp, options);
}
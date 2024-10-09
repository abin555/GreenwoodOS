#include "libc.h"
#define DOOM_IMPLEMENTATION 
#include "PureDOOM.h"

#define WIDTH 320
#define HEIGHT 200
#define SCALE 4

int running;

struct ViewportFunctions *vp_funcs;
struct Viewport *window;

uint32_t *private_region;
void *malloc_walker;

void *memcpy(void *dest, const void *src, int n){
  for(int i = 0; i < n; i++){
    ((uint8_t *) dest)[i] = ((uint8_t *) src)[i];
  }
}

void *malloc_impl(int size){
    void *addr = malloc_walker;
    malloc_walker += size;
    return addr;
}

void free_impl(void *ptr){
    return;
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
  switch(event){
    case VP_EXIT:
      running = 0;
      break;
    case VP_MAXIMIZE:
    //case VP_FOCUSED:
      set_schedule(ALWAYS);
      break;
    case VP_MINIMIZE:
    //case VP_UNFOCUSED:
      set_schedule(NEVER);
      break;
  }
}

char *getenv(const char *var){
    if(!strcmp(var, "HOME")){
        return "/A/DOOM/";
    }
}

int main(int argc, char **argv){
    private_region = requestRegion(0x10000);
    malloc_walker = private_region;

    doom_set_malloc(malloc_impl, free_impl);
    doom_set_exit(exit);
    doom_set_print((void (*)(const char *)) print);
    doom_set_getenv(getenv);

    running = 1;
    vp_funcs = viewport_get_funcs();
    window = vp_funcs->open(WIDTH, HEIGHT, "DOOM");
    vp_funcs->add_event_handler(window, event_handler);
    print("DOOM starting init\n");
    doom_init(argc, argv, 0);

    while(running){
        doom_update();
        uint32_t* framebuffer = (uint32_t *) doom_get_framebuffer(4 /* RGBA */);
        vp_funcs->set_buffer(window, framebuffer, WIDTH * HEIGHT * SCALE);
        vp_funcs->copy(window);
    }


    vp_funcs->close(window);
}
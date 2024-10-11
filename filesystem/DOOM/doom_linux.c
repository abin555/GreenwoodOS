#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#define DOOM_IMPLEMENT_PRINT
#define DOOM_IMPLEMENT_MALLOC
#define DOOM_IMPLEMENT_FILE_IO
#define DOOM_IMPLEMENT_GETTIME
#define DOOM_IMPLEMENT_EXIT
#define DOOM_IMPLEMENT_GETENV
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

int main(int argc, char **argv){
  doom_set_print((void (*)(const char *)) printf);
  doom_print("DOOM starting init\n");
  doom_init(argc, argv, 0);

  running = 1;
  uint32_t* framebuffer;

  while(running){
    doom_update();
    framebuffer = (uint32_t *) doom_get_framebuffer(4 /* RGBA */);
  }
}
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

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

void *private_region;
void *malloc_walker;

void *malloc_impl(int size){
  doom_print("MALLOC for ");
  doom_print(doom_itoa(size, 10));
  size += 0x10;
  void *addr = malloc_walker;
  malloc_walker += size;
  doom_memset(addr, 0, size);
  doom_print(" Walker at ");
  doom_print(doom_ptoa(malloc_walker));
  doom_print("\n");
  return addr;
}

void free_impl(void *ptr){
    return;
}

int main(int argc, char **argv){
  private_region = malloc(2*0x400000);
  malloc_walker = private_region;

  doom_set_malloc(malloc_impl, free_impl);
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
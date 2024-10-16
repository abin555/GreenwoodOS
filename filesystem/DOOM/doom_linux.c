#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

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
  doom_print("\n");
  return malloc(size);
}

void free_impl(void *ptr){
  return free(ptr);
}

void *impl_fopen(const char *filename, const char *mode){
  return fopen(filename, mode);
}

void impl_fclose(void *handle){
  fclose((FILE *) handle);
}

int impl_fread(void *handle, void *buf, int count){
  fread(buf, 1, count, (FILE *) handle);
  
  doom_print("File Read for");
  doom_print(doom_itoa(count, 10));
  doom_print("\n");
  for(int i = 0; i < count; i++){
    char a = ((char *) buf)[i];
    printf("%c", a);
  }
  doom_print("\nFile Read Done\n");
  
  return count;
}

int impl_fwrite(void *handle, const void *buf, int count){
  return 0;
}

int impl_fseek(void *handle, int offset, doom_seek_t origin){
  switch(origin){
    case DOOM_SEEK_CUR:
    fseek((FILE *) handle, offset, SEEK_CUR);
    break;
    case DOOM_SEEK_END:
    fseek((FILE *) handle, offset, SEEK_END);
    break;
    case DOOM_SEEK_SET:
    fseek((FILE *) handle, offset, SEEK_SET);
    break;
  }
  return 0;
}

int impl_ftell(void *handle){
  return ftell((FILE *) handle);
}

int impl_feof(void *handle){
  return feof((FILE *) handle);
  return 0;
}

int main(int argc, char **argv){
  private_region = malloc(2*0x400000);
  malloc_walker = private_region;

  doom_set_malloc(malloc_impl, free_impl);
  doom_set_print((void (*)(const char *)) printf);
  doom_set_file_io(
    impl_fopen,
    impl_fclose,
    impl_fread,
    impl_fwrite,
    impl_fseek,
    impl_ftell,
    impl_feof
  );
  
  doom_print("DOOM starting init\n");
  doom_init(argc, argv, 0);

  running = 1;
  uint32_t* framebuffer;
  printf("Startup\n");
  while(running){
    doom_update();
    framebuffer = (uint32_t *) doom_get_framebuffer(4 /* RGBA */);
  }
}
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
  return dest;
}

void *malloc_impl(int size){
  doom_print("MALLOC for ");
  doom_print(doom_itoa(size, 10));
  doom_print("\n");
  void *addr = malloc_walker;
  malloc_walker += size;

  doom_memset(addr, 0, size);
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
      return "/A/DOOM";
  }
  else if(!strcmp(var, "DOOMWADDIR")){
    return "/A/DOOM";
  }
  else{
    print("UNKNOWN ENV: ");
    print(var);
    print("\n");
  }
  return NULL;
}

void *impl_fopen(const char *filename, const char *mode){
  if(!fexists(filename)) return NULL;
  struct FILE *file = fopen(filename);
  return file;
}

void impl_fclose(void *handle){
  fclose((struct FILE *) handle);
}

int impl_fread(void *handle, void *buf, int count){
  fcopy(handle, buf, count);
  return count;
}

int impl_fwrite(void *handle, const void *buf, int count){
  char *array = (char *) buf;
  for(int i = 0; i < count; i++){
    fputc(handle, array[i]);
  }
  return count;
}

int impl_fseek(void *handle, int offset, doom_seek_t origin){
  switch(origin){
    case DOOM_SEEK_CUR:
    fseek(handle, ((struct FILE *) handle)->head + offset);
    break;
    case DOOM_SEEK_END:
    fseek(handle, fsize(handle) + offset);
    break;
    case DOOM_SEEK_SET:
    fseek(handle, offset);
    break;
  }
  return 0;
}

int impl_ftell(void *handle){
  return ((struct FILE *) handle)->head;
}

int impl_feof(void *handle){
  if(((struct FILE *)handle)->head >= fsize(handle)) return 1;
  return 0;
}

struct RealTimeClock *rtc;

void impl_gettime(int* sec, int* usec){
  *sec = rtc->second;
  *usec = 0;
}

int main(int argc, char **argv){
  private_region = requestRegion(2*0x800000);
  malloc_walker = private_region;
  rtc = get_rtc();

  print_serial("Z_Free is at 0x");
  print_serial(doom_itoa((unsigned int) Z_Free, 16));
  print_serial("\n");

  doom_set_malloc(malloc_impl, free_impl);
  doom_set_exit(exit);
  doom_set_print((void (*)(const char *)) print_serial);
  doom_set_getenv(getenv);
  doom_set_file_io(
    impl_fopen,
    impl_fclose,
    impl_fread,
    impl_fwrite,
    impl_fseek,
    impl_ftell,
    impl_feof
  );
  doom_set_gettime(impl_gettime);

  print("DOOM starting init\n");
  doom_init(argc, argv, 0);

  running = 1;
  vp_funcs = viewport_get_funcs();
  window = vp_funcs->open(WIDTH, HEIGHT, "DOOM");
  vp_funcs->add_event_handler(window, event_handler);

  uint32_t* framebuffer;

  while(running){
    doom_update();
    framebuffer = (uint32_t *) doom_get_framebuffer(4 /* RGBA */);
    vp_funcs->set_buffer(window, framebuffer, WIDTH * HEIGHT * SCALE);
    vp_funcs->copy(window);
  }


  vp_funcs->close(window);
}
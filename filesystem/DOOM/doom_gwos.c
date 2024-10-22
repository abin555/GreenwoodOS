#include "libc.h"
//#define DOOM_IMPLEMENTATION
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
  void *addr = malloc_walker;
  malloc_walker += size;

  doom_memset(addr, 0, size);
  return addr;
}

void free_impl(void *ptr){
    return;
}

struct KeyData{
  char current_c;
  char previous_c;
};

struct KeyData keyData;

char key_pressed_map_back[0xFF];
char *key_pressed_map;

int skips;
void handle_key(){
  if(skips < 3){
    skips++;
    return;
  }
  skips = 0;
  for(int i = 0; i < 0xFF; i++){
    int key_current = key_pressed_map[i];
    int key_prev = key_pressed_map_back[i];

    if(key_current != 0 && key_prev == 0){
      switch((char) i){
        case 'a':
          doom_key_down(DOOM_KEY_LEFT_ARROW);
          break;
        case 'd':
          doom_key_down(DOOM_KEY_RIGHT_ARROW);
          break;
        case 'w':
          doom_key_down(DOOM_KEY_UP_ARROW);
          break;
        case 's':
          doom_key_down(DOOM_KEY_DOWN_ARROW);
          break;
        case 'e':
          doom_key_down(DOOM_KEY_SPACE);
          break;
        case 10:
          doom_key_down(DOOM_KEY_ENTER);
          break;
        case 8:
          doom_key_down(DOOM_KEY_BACKSPACE);
          break;
        case ' ':
          doom_key_down(DOOM_KEY_CTRL);
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case ',':
        case '.':
          doom_key_down(i);
          break;
      }
    }
    else if(key_current == 0 && key_prev != 0){
      switch((char) i){
        case 'a':
          doom_key_up(DOOM_KEY_LEFT_ARROW);
          break;
        case 'd':
          doom_key_up(DOOM_KEY_RIGHT_ARROW);
          break;
        case 'w':
          doom_key_up(DOOM_KEY_UP_ARROW);
          break;
        case 's':
          doom_key_up(DOOM_KEY_DOWN_ARROW);
          break;
        case 'e':
          doom_key_up(DOOM_KEY_SPACE);
          break;
        case 10:
          doom_key_up(DOOM_KEY_ENTER);
          break;
        case 8:
          doom_key_up(DOOM_KEY_BACKSPACE);
          break;
        case ' ':
          doom_key_up(DOOM_KEY_CTRL);
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case ',':
        case '.':
          doom_key_up(i);
          break;
      }
    }
  }

  doom_memcpy(key_pressed_map_back, key_pressed_map, sizeof(key_pressed_map_back));
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
  switch(event){
    case VP_EXIT:
      running = 0;
      break;
    case VP_MAXIMIZE:
    case VP_FOCUSED:
      set_schedule(ALWAYS);
      break;
    case VP_MINIMIZE:
    case VP_UNFOCUSED:
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

unsigned int ticks = 0;

void impl_gettime(int* sec, int* usec){  
  *sec = 60*rtc->minute + rtc->second;
  *usec = rtc->msec;
  //ticks++;
  //*sec = ticks / 1000;
  //*usec = ticks % 1000;
}

void impl_exit(int code){
  running = 0;
  vp_funcs->close(window);
  freeRegion(private_region, 2*0x800000);
  exit(code);
}

int main(int argc, char **argv){
  private_region = requestRegion(2*0x800000);
  malloc_walker = private_region;
  rtc = get_rtc();

  struct FEATURE_INFO keypressed_feature = getKernelFeature(FEAT_KEYPRESSMAP);
  key_pressed_map = keypressed_feature.addr;

  doom_set_malloc(malloc_impl, free_impl);
  doom_set_exit(impl_exit);
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
  task_lock(1);
  doom_init(argc, argv, 0);
  task_lock(0);

  running = 1;
  vp_funcs = viewport_get_funcs();
  window = vp_funcs->open(WIDTH, HEIGHT, "DOOM");
  window->loc.x = 400-160;
  window->loc.y = 300-120;
  vp_funcs->add_event_handler(window, event_handler);
  //addEndCallback(end_callback);

  uint32_t* framebuffer;

  while(running){
    task_lock(1);
    doom_update();
    task_lock(0);
    handle_key();
    framebuffer = (uint32_t *) doom_get_framebuffer(4 /* RGBA */);
    vp_funcs->set_buffer(window, framebuffer, WIDTH * HEIGHT * SCALE);
    vp_funcs->copy(window);
    for(int i = 0; i < 0x8FFFFF; i++){}
  }

  vp_funcs->close(window);
  freeRegion(private_region, 2*0x800000);
}
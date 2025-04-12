#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/memory.h>
//#define DOOM_IMPLEMENTATION
#include "PureDOOM.h"

#define WIDTH 320
#define HEIGHT 200
#define SCALE 4

int running;

struct ViewportFunctions *vp_funcs;
struct Viewport *window;

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
    printf("UNKNOWN ENV: %s\n", var);
  }
  return NULL;
}

void *impl_fopen(const char *filename, const char *mode){
  return fopen(filename, mode);
}

void impl_fclose(void *handle){
  fclose((struct FILE *) handle);
}

int impl_fread(void *handle, void *buf, int count){
  return fread(buf, count, 1, (FILE *) handle);
}

int impl_fwrite(void *handle, const void *buf, int count){
  return fwrite(buf, count, 1, (FILE *) handle);
}

int impl_fseek(void *handle, int offset, doom_seek_t origin){
  switch(origin){
    case DOOM_SEEK_CUR:
    return fseek((FILE *) handle, offset, SEEK_CUR);
    break;
    case DOOM_SEEK_END:
    return fseek((FILE *) handle, offset, SEEK_END);
    break;
    case DOOM_SEEK_SET:
    return fseek((FILE *) handle, offset, SEEK_SET);
    break;
  }
  return 0;
}

int impl_ftell(void *handle){
  return ftell((FILE *) handle);
}

int impl_feof(void *handle){
  return feof((FILE *) handle);
}

struct RealTimeClock {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned int msec;
};

int rtc_fd;

unsigned int ticks = 0;

void impl_gettime(int* sec, int* usec){ 
  struct RealTimeClock rtc;
  read(rtc_fd, &rtc, sizeof(rtc));
  *sec = 60*rtc.minute + rtc.second;
  *usec = rtc.msec;
  //ticks++;
  //*sec = ticks / 1000;
  //*usec = ticks % 1000;
}

void impl_exit(int code){
  running = 0;
  vp_close(window);
  exit(code);
}

int main(int argc, char **argv){
  struct FEATURE_INFO keypressed_feature = getKernelFeature(FEAT_KEYPRESSMAP);
  key_pressed_map = keypressed_feature.addr;
  rtc_fd = open("/-/dev/RTC", O_READ);
  if(rtc_fd == -1){
    printf("Unable to access clock!\nExiting\n");
    return 1;
  }
  freopen("/-/dev/serial", "a+", stdout);

  doom_set_malloc((doom_malloc_fn) malloc, (doom_free_fn) free);
  doom_set_exit(impl_exit);
  doom_set_print((void (*)(const char *)) puts);
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

  printf("DOOM starting init\n\n\0");
  task_lock(1);
  doom_init(argc, argv, 0);
  task_lock(0);

  running = 1;
  window = vp_open(WIDTH, HEIGHT, "DOOM");
  window->loc.x = 400-160;
  window->loc.y = 300-120;
  vp_add_event_handler(window, event_handler);
  //addEndCallback(end_callback);

  uint32_t* framebuffer;

  while(running){
    task_lock(1);
    doom_update();
    task_lock(0);
    handle_key();
    framebuffer = (uint32_t *) doom_get_framebuffer(4 /* RGBA */);
    vp_set_buffer(window, framebuffer, WIDTH * HEIGHT * SCALE);
    vp_copy(window);
    for(int i = 0; i < 0x8FFFFF; i++){}
  }

  vp_close(window);
  close(rtc_fd);
}
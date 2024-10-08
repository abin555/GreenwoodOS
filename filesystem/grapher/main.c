#include "libc.h"

#define PI 3.1415
#define PI2 6.283
#define WIDTH 300
#define HEIGHT 300

#define BUF_SIZE WIDTH * HEIGHT * sizeof(uint32_t)

#define ED_WIDTH 300
#define ED_HEIGHT 100
#define EDITOR_BUF_SIZE ED_WIDTH * ED_HEIGHT * sizeof(uint32_t) 
struct ViewportFunctions *vp_funcs;
struct Viewport *window;
struct Viewport *editor_window;
uint32_t *win_buf;
uint32_t *editor_window_buf;

int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);
void editor_event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);
void memset(void *mem, char v, int size);

struct Plot{
    float Xmin;
    float Xmax;
    float Ymin;
    float Ymax;
};

struct Vec2{
    float x;
    float y;
};
struct Plot plot;

void drawPoint(struct Plot *plot, struct Vec2 point, uint32_t color);
float f(float x);


void drawPoint(struct Plot *plot, struct Vec2 point, uint32_t color){
    if(plot == NULL) return;
    if(point.x < plot->Xmin || point.x > plot->Xmax || point.y < plot->Ymin || point.y > plot->Ymax) return;
    float normalX = (point.x - plot->Xmin) / (plot->Xmax - plot->Xmin);
    float normalY = (point.y - plot->Ymin) / (plot->Ymax - plot->Ymin);
    int screenX = (int)(normalX * WIDTH);
    int screenY = HEIGHT - (int)(normalY * HEIGHT);
    win_buf[screenY*WIDTH+screenX] = color;
}

int main(int argc, char **argv){
  plot.Xmin = -10;
  plot.Xmax = 10;
  plot.Ymin = -10;
  plot.Ymax = 10;
  running = 1;
  vp_funcs = viewport_get_funcs();
  win_buf = (uint32_t *) 0x6000;
  editor_window_buf = (uint32_t *) (((char *) win_buf) + BUF_SIZE);
  window = vp_funcs->open(WIDTH, HEIGHT, "Grapher");
  editor_window = vp_funcs->open(ED_WIDTH, ED_HEIGHT, "Grapher Editor");
  editor_window->click_events_enabled = true;
  vp_funcs->add_event_handler(window, event_handler);
  vp_funcs->add_event_handler(editor_window, editor_event_handler);
  vp_funcs->set_buffer(window, win_buf, BUF_SIZE);
  vp_funcs->set_buffer(editor_window, editor_window_buf, EDITOR_BUF_SIZE);

  set_schedule(ALWAYS);
  memset(editor_window_buf, 0x22, EDITOR_BUF_SIZE);
  vp_funcs->copy(editor_window);

  while(running){
    memset(win_buf, 0x22, BUF_SIZE);
    for(float x = plot.Xmin; x<=plot.Xmax; x+=0.01){
      struct Vec2 p = {x, 0};
      drawPoint(&plot, p, 0xFFFFFF);
      p.x = 0;
      p.y = x;
      drawPoint(&plot, p, 0xFFFFFF);
    }

    for(float x = plot.Xmin; x <= plot.Xmax; x+=0.05){
      struct Vec2 p = {x, f(x)};
      drawPoint(&plot, p, 0xFF0000);
    }
    vp_funcs->copy(window);
  }
  vp_funcs->close(window);
  vp_funcs->close(editor_window);
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

void editor_event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
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
    case VP_CLICK:
      plot.Xmin++;
      break;
  }
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float cos(float x)
{
  x = modd(x, PI2);
  char sign = 1;
  if (x > PI)
  {
      x -= PI;
      sign = -1;
  }
  return sign * (1 - ((x * x) / (2)) + ((x * x * x * x) / (24)) - ((x * x * x * x * x * x) / (720)) + ((x * x * x * x * x * x * x * x) / (40320)) - ((x * x * x * x * x * x * x * x * x * x) / (3628800)) + ((x * x * x * x * x * x * x * x * x * x * x * x) / (479001600)));
}

float sin(float x){
  return cos(x - (PI / 2));
}

float f(float x){
    //return x*x - 2.0f;
    return sin(x);
    //return (x*x*x - 5*x*x + 4*x + 2);
}
#include "libc.h"

#define PI 3.1415
#define PI2 6.283
#define SCALE 150

#define WIDTH 150
#define HEIGHT 150
#define BUF_SIZE WIDTH * HEIGHT * sizeof(uint32_t)

struct ViewportFunctions *vp_funcs;
struct Viewport *cube_vp;
uint32_t *cube_buf;

struct WINDOW *window;
uint32_t *win_buf;

const int zOff = 250;
const int xOff = 10;
const int yOff = 10;
const int cSize = 50;
const int view_plane = 64;
const float angle = PI/60;

float cube3d[8][3] = {
  {xOff - cSize,yOff + cSize,zOff - cSize},
  {xOff + cSize,yOff + cSize,zOff - cSize},
  {xOff - cSize,yOff - cSize,zOff - cSize},
  {xOff + cSize,yOff - cSize,zOff - cSize},
  {xOff - cSize,yOff + cSize,zOff + cSize},
  {xOff + cSize,yOff + cSize,zOff + cSize},
  {xOff - cSize,yOff - cSize,zOff + cSize},
  {xOff + cSize,yOff - cSize,zOff + cSize}
};
unsigned char cube2d[8][2];


void memset(void *mem, char v, int size);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
float sin(float x);
float cos(float x);

void printcube();
void zrotate(float q);
void yrotate(float q);
void xrotate(float q);
void draw_cube();

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

int running;

int main(int argc, char **argv){
  print("Opening Math Renderer\n");
  uint32_t *ticks = getTimerTickHandle();
  vp_funcs = viewport_get_funcs();
  cube_buf = (uint32_t *) 0x6000;

  cube_vp = vp_funcs->open(WIDTH, HEIGHT, "Cube");
  vp_funcs->add_event_handler(cube_vp, event_handler);
  vp_funcs->set_buffer(cube_vp, cube_buf, BUF_SIZE);
  
  set_schedule(ALWAYS);
  srand(1234);

  float angle = 0;
  
  int centerX = WIDTH / 2;
  int centerY = HEIGHT / 2;

  running = 1;

  while(running){    
    for(int i = 0; i < 0xFF; i++){
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
    }
    int rsteps = rand() % 10;
    switch(rand() % 6) {
      case 0:
        for (int i = 0; i < rsteps; i++) {
          zrotate(-0.1);
          memset(cube_buf, 0, BUF_SIZE);
          printcube();
          vp_funcs->copy(cube_vp);
        }
        break;
      case 1:
        for (int i = 0; i < rsteps; i++) {
          zrotate(0.1);
          memset(cube_buf, 0, BUF_SIZE);
          printcube();
          vp_funcs->copy(cube_vp);
        }
        break;
      case 2:
        for (int i = 0; i < rsteps; i++) {
          xrotate(-0.1);
          memset(cube_buf, 0, BUF_SIZE);
          printcube();
          vp_funcs->copy(cube_vp);
        }
        break;
      case 3:
        for (int i = 0; i < rsteps; i++) {
          xrotate(0.1);
          memset(cube_buf, 0, BUF_SIZE);
          printcube();
          vp_funcs->copy(cube_vp);
        }
        break;
      case 4:
        for (int i = 0; i < rsteps; i++) {
          yrotate(-0.1);
          memset(cube_buf, 0, BUF_SIZE);
          printcube();
          vp_funcs->copy(cube_vp);
        }
        break;
      case 5:
        for (int i = 0; i < rsteps; i++) {
          yrotate(0.1);
          memset(cube_buf, 0, BUF_SIZE);
          printcube();
          vp_funcs->copy(cube_vp);
        }
        break;
    }
    
    
    //printcube();
    vp_funcs->copy(cube_vp);
  }
  vp_funcs->close(cube_vp);
}

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float cos(float x)
{
  x = modd(x, PI2);
  return 1 - ((x * x) / (2)) + ((x * x * x * x) / (24)) - ((x * x * x * x * x * x) / (720)) + ((x * x * x * x * x * x * x * x) / (40320)) - ((x * x * x * x * x * x * x * x * x * x) / (3628800)) + ((x * x * x * x * x * x * x * x * x * x * x * x) / (479001600));
}

float sin(float x){
  return cos(x - (PI / 2));
}

/*
plotLine(x0, y0, x1, y1)
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy

void
plot_line (int x0, int y0, int x1, int y1)
{
  int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; 
 
  for (;;){
    setPixel (x0,y0);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}
*/

int abs(int x){
  if(x < 0) return x * -1;
  return x;
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color){
  /*
    if(x1 == x2){
        for(int y = y1; y < y2; y++){
            win_buf[x1 + window->width*y] = color;
        }
        return;
    }

    int dx = x2 - x1;
    int dy = y2 - y1;
    int D = 2*dy - dx;
    int y = y1;

    for(int x = x1; x < x2; x++){
        win_buf[x + window->width*y] = color;
        if(D > 0){
            y = y + 1;
            D = D - 2*dx;
        }
        D = D + 2*dy;
    }
  */
  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for(;;){
    cube_buf[x1 + y1 * WIDTH] = color;
    if(x1 == x2 && y1 == y2) break;
    e2 = 2 * err;
    if(e2 >= dy){err += dy; x1 += sx;}
    if(e2 <= dx){err += dx; y1 += sy;}
  }
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}

void printcube() {
  //calculate 2d points
  for(unsigned char i = 0; i < 8; i++) {
    cube2d[i][0] = (unsigned char)((cube3d[i][0] * view_plane / cube3d[i][2]) + (WIDTH/2));
    cube2d[i][1] = (unsigned char)((cube3d[i][1] * view_plane / cube3d[i][2]) + (HEIGHT/2));
  }
  //TV.delay_frame(1);
  draw_cube();
}

void zrotate(float q) {
  float tx,ty,temp;
  for(unsigned char i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    ty = cube3d[i][1] - yOff;
    temp = tx * cos(q) - ty * sin(q);
    ty = tx * sin(q) + ty * cos(q);
    tx = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][1] = ty + yOff;
  }
}

void yrotate(float q) {
  float tx,tz,temp;
  for(unsigned char i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    tz = cube3d[i][2] - zOff;
    temp = tz * cos(q) - tx * sin(q);
    tx = tz * sin(q) + tx * cos(q);
    tz = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][2] = tz + zOff;
  }
}

void xrotate(float q) {
  float ty,tz,temp;
  for(unsigned char i = 0; i < 8; i++) {
    ty = cube3d[i][1] - yOff;
    tz = cube3d[i][2] - zOff;
    temp = ty * cos(q) - tz * sin(q);
    tz = ty * sin(q) + tz * cos(q);
    ty = temp;
    cube3d[i][1] = ty + yOff;
    cube3d[i][2] = tz + zOff;
  }
}

void draw_cube() {
  drawLine(cube2d[0][0],cube2d[0][1],cube2d[1][0],cube2d[1][1],0xFF0000);
  drawLine(cube2d[0][0],cube2d[0][1],cube2d[2][0],cube2d[2][1],0xFF0000);
  drawLine(cube2d[0][0],cube2d[0][1],cube2d[4][0],cube2d[4][1],0xFF0000);
  drawLine(cube2d[1][0],cube2d[1][1],cube2d[5][0],cube2d[5][1],0xFF0000);
  drawLine(cube2d[1][0],cube2d[1][1],cube2d[3][0],cube2d[3][1],0xFF0000);
  drawLine(cube2d[2][0],cube2d[2][1],cube2d[6][0],cube2d[6][1],0xFF0000);
  drawLine(cube2d[2][0],cube2d[2][1],cube2d[3][0],cube2d[3][1],0xFF0000);
  drawLine(cube2d[4][0],cube2d[4][1],cube2d[6][0],cube2d[6][1],0xFF0000);
  drawLine(cube2d[4][0],cube2d[4][1],cube2d[5][0],cube2d[5][1],0xFF0000);
  drawLine(cube2d[7][0],cube2d[7][1],cube2d[6][0],cube2d[6][1],0xFF0000);
  drawLine(cube2d[7][0],cube2d[7][1],cube2d[3][0],cube2d[3][1],0xFF0000);
  drawLine(cube2d[7][0],cube2d[7][1],cube2d[5][0],cube2d[5][1],0xFF0000);
  //delay(500);
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
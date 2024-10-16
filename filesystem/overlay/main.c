#include "libc.h"

#define PI 3.1415
#define PI2 6.283
#define SCALE 10

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float cos(float x)
{
  x = modd(x, PI2);
  return 1 - ((x * x) / (2)) + ((x * x * x * x) / (24)) - ((x * x * x * x * x * x) / (720)) + ((x * x * x * x * x * x * x * x) / (40320)) - ((x * x * x * x * x * x * x * x * x * x) / (3628800)) + ((x * x * x * x * x * x * x * x * x * x * x * x) / (479001600));
}

float sin(float x){
  return cos(x - (PI / 2));
}

int abs(int x){
  if(x < 0) return x * -1;
  return x;
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color, uint32_t *buf, uint32_t buf_width){
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

  if(
    x1 < 0 || x2 < 0 || x1 >= 800 || x2 >= 800 ||
    y1 < 0 || y2 < 0 || y1 >= 600 || y2 >= 600
  ){
    exit(1);
  }

  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for(;;){
    buf[x1 + y1 * buf_width] = color;
    if(x1 == x2 && y1 == y2) break;
    e2 = 2 * err;
    if(e2 >= dy){err += dy; x1 += sx;}
    if(e2 <= dx){err += dx; y1 += sy;}
  }
}

void drawCircle(float *angle, int centerX, int centerY, uint32_t *buf, uint32_t buf_width){
  int xReal;
  int yReal;
  *angle += 0.1;
  if(*angle >= PI) *angle = -PI;
  float x = cos(*angle);
  float y = sin(*angle);
  
  xReal = x*SCALE + centerX;
  yReal = y*SCALE + centerY;
  drawLine(centerX, centerY, xReal, yReal, 0xFFFFFF, buf, buf_width);
}

char **global_argv;

void end_callback();

int main(int argc, char **argv){
    struct FEATURE_INFO framebuffer = getKernelFeature(FEAT_FRAMEBUFFER);
    uint32_t *fb = framebuffer.addr;
    global_argv = argv;

    addEndCallback(end_callback);

    float angle = 0;
    while(1){
        struct MouseStatus mouse = getMouse();
        drawCircle(&angle, mouse.pos.x, mouse.pos.y, fb, 800);
        //drawLine(400,300,mouse.pos.x,mouse.pos.y, 0xFFFFFF, fb, 800);
    }
}

void end_callback(){
    exec(global_argv[0], 1, global_argv);
}

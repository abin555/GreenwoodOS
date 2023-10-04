#include "libc.h"

#define PI 3.1415
#define PI2 6.283
#define SCALE 150

struct WINDOW *window;
uint32_t *win_buf;

void memset(void *mem, char v, int size);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
float sin(float x);
float cos(float x);

int main(int argc, char **argv){
    print("Opening Math Renderer\n");
    window = window_open("Math Renderer");
    win_buf = window->backbuffer;
    
    window_update();
    set_schedule(ONFOCUS);
    srand(1234);

    float angle = 0;
    float deltaAngle = 0.1;
    float x, y;
    int xReal, yReal;
    
    int centerX = window->width / 2;
    int centerY = window->height / 2;

    while(1){
        angle += deltaAngle;
        if(angle >= PI) angle = -PI;
        x = cos(angle);
        y = sin(angle);
        
        xReal = x*SCALE + centerX;
        yReal = y*SCALE + centerY;
        
	      memset(win_buf, 0, window->width*window->height*sizeof(uint32_t));

        //win_buf[xReal + yReal*window->width] = 0xFFFFFF;
        drawChar(xReal, yReal, '0');
        drawLine(centerX, centerY, xReal, yReal, 0xFFFFFF);

        window_update();
    }
}

/*
float sin(float x){
  return __builtin_sin(x);
}


float cos(float x)
{
  return __builtin_cos(x);
}
*/

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
    win_buf[x1 + y1 * window->width] = color;
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


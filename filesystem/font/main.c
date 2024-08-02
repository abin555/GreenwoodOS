#include "libc.h"

struct FEATURE_INFO FONT_INFO;

uint32_t *win_buf;
struct WINDOW *window;
//char (*FONT)[128][8];
typedef char FONT_T[128][8];
FONT_T *FONT;

void render_font();
void drawPixelScaled(int x, int y, int size, uint32_t color);
void drawCharBig(int idx, int size, int x, int y);

#define FontScale 12
#define IndicatorScale 2

int main(int argc, char *argv[]){
    print("Opening Font Editor\n");
    window = window_open("FONTED", 0);
    win_buf = window->backbuffer;

    window_update();
    set_schedule(ONFOCUS);

    FONT_INFO = getKernelFeature(FEAT_FONT);
    print_arg("Font is at memory %x\n", (uint32_t) FONT_INFO.addr);

    FONT = (char (*)[128][8]) FONT_INFO.addr;

    //render_font();
    //drawPixelScaled(3,3,8,0xFFFFFF);
    
    char letter = 0;
    char input = 0;
    int x = 0;
    int y = 0;
    while(1){
        for(int i = 0; i < 10; i++){
            drawPixelScaled(i, 0, FontScale, 0xFF0000);
        }
        for(int i = 0; i < 10; i++){
            drawPixelScaled(i, 9, FontScale, 0xFF0000);
        }
        for(int i = 0; i < 10; i++){
            drawPixelScaled(0, i, FontScale, 0xFF0000);
        }
        for(int i = 0; i < 10; i++){
            drawPixelScaled(9, i, FontScale, 0xFF0000);
        }
        drawCharBig(letter, FontScale, 1, 1);
        drawPixelScaled(IndicatorScale + x*IndicatorScale, IndicatorScale + y*IndicatorScale, FontScale/IndicatorScale, 0x4040FF);
        //window_update();

        input = getc();

        if(input == 'q' || input == 'Q'){
            letter--;
        }
        else if(input == 'e' || input == 'E'){
            letter++;
        }
        if(input == 'w'){
            y--;
            if(y<0) y = 7;
        }
        if(input == 's'){
            y++;
            if(y>7) y = 0;
        }
        if(input == 'a'){
            x--;
            if(x<0) x = 7;
        }
        if(input == 'd'){
            x++;
            if(x>7) x = 0;
        }
        if(input == ' '){
            char current = (((*FONT)[letter][y] >> x) & 1);
            char mask;
            if(current){
                mask = ~(1 << x);
                (*FONT)[letter][y] &= mask;
            }
            else{
                mask = 1 << x;
                (*FONT)[letter][y] |= mask;
            }
        }
    }
}
    

void drawCharGrid(int x, int y, int size){
    int Gx, Gy;
    Gx = x;
    Gy = y;
    //Horizontal Lines
    for(Gx = x; Gx < x + size + 1; Gx++){
        win_buf[Gx + Gy * window->width] = 0xFFFFFF;
    }
}

void drawPixelScaled(int x, int y, int size, uint32_t color){
    x *= size;
    y *= size;
    for(int dx = 0; dx < size; dx++){
        for(int dy = 0; dy < size; dy++){
            win_buf[x+dx + (y+dy)*window->width] = color;
        }
    }
}

void drawCharBig(int idx, int size, int x, int y){
    for(int layer = 0; layer < 8; layer++){
        for(int pixel = 0; pixel < 8; pixel++){
            //win_buf[window->width*(y+layer) + x+pixel] = (((*FONT)[(int)i][layer] >> pixel) & 1) ? 0xFFFFFF : 0;
            /*
            if(((*FONT)[idx][layer] >> pixel) & 1){
                drawPixelScaled(x+pixel,y+layer, size, 0xFFFFFF);
            }
            else{
                drawPixelScaled(x+pixel,y+layer, size, 0);
            }
            */
            drawPixelScaled(x+pixel,y+layer, size, (((*FONT)[(int)idx][layer] >> pixel) & 1) ? 0xFFFFFF : 0);
        }
    }
}

void render_font(){
    int x, y;
    x = 0;
    y = 0;
    for(int i = 0; i < 128; i++){
        for(int layer = 0; layer < 8; layer++){
            for(int pixel = 0; pixel < 8; pixel++){
                win_buf[window->width*(y+layer) + x+pixel] = (((*FONT)[(int)i][layer] >> pixel) & 1) ? 0xFFFFFF : 0;
            }
        }
        x += 8;
        if(x >= window->width - 8){
            x = 0;
            y += 8;
        }
    }
}
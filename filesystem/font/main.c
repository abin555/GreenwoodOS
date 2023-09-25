#include "libc.h"

struct FEATURE_INFO FONT_INFO;

uint32_t *win_buf;
struct WINDOW *window;
//char (*FONT)[128][8];
typedef char FONT_T[128][8];
FONT_T *FONT;

void render_font();

int main(int argc, char *argv[]){
    print("Opening Font Editor\n");
    window = window_open("FONTED");
    win_buf = window->backbuffer;

    window_update();
    set_schedule(ONFOCUS);

    FONT_INFO = getKernelFeature(FEAT_FONT);
    print_arg("Font is at memory %x\n", (uint32_t) FONT_INFO.addr);

    FONT = (char (*)[128][8]) FONT_INFO.addr;

    render_font();
    window_update();

    while(1){

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
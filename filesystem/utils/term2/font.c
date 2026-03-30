#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "font.h"

typedef char FONT_T[128][8];
FONT_T *FONT;

int font_load(){
    FILE *font_file = fopen("/-/sys/font", "r");
    if(font_file == NULL) return 1;
    FONT = malloc(sizeof(FONT_T));
    fread(FONT, sizeof(FONT_T), 1, font_file);
    fclose(font_file);
}

void font_drawChar(uint32_t *buf, uint32_t width, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, char c){
    for(int layer = 0; layer < 8; layer++){
        for(int pixel = 0; pixel < 8; pixel++){
            buf[x+pixel+((y+layer)*width)] = (((*FONT)[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}
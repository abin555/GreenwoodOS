#include "gfx.h"


void drawRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for(int x = x1; x <= x2; x++){
        for(int y = y1; y <= y2; y++){
            buf[y*buf_width + x] = buf[y*buf_width + x] | innerColor;
            if(x == x1 || y == y1 || x == x2 || y == y2){
                buf[y*buf_width + x] = outerColor;
            }
        }
    }
}

void fillRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if(x2 >= (int) buf_width) x2 = buf_width - 1;

    for(int x = x1; x <= x2; x++){
        for(int y = y1; y <= y2; y++){
            buf[y*buf_width + x] = innerColor;
            if(x == x1 || y == y1 || x == x2 || y == y2){
                buf[y*buf_width + x] = outerColor;
            }
        }
    }
}
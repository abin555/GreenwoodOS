#include "block.h"

void block_draw(uint8_t x, uint8_t y, uint32_t *buf, uint8_t mode){
    for(uint8_t i = 0; i < 4; i++){
        for(uint8_t j = 0; j < 4; j++){
            if(mode == 0){
                if((i == 0 || i == 3) || (j == 0 || j == 3)){
                    buf[x+i+(y+j)*64] = 0xFFFFFF;
                }
            }
            else{
                if(!((i == 0 || i == 3) || (j == 0 || j == 3))){
                    buf[x+i+(y+j)*64] = 0xFFFFFF;
                }
            }
        }
    }
}

void draw_outline(uint32_t *buf){
    const uint8_t yoff = 128 - 4*22;
    const uint8_t xoff = 4*2;
    for(uint8_t x = 0; x < 12; x++){
        for(uint8_t y = 0; y < 22; y++){
            if((x == 0 || x == 11) || (y == 0 || y == 21)){
                block_draw(
                    xoff+x*4, yoff+(y*4),
                    buf,
                    0
                );
            }
        }
    }
}

void invert_screen(uint32_t *buf){
    for(int i = 0; i < 128*64; i++){
        buf[i] = ~buf[i];
    }
}
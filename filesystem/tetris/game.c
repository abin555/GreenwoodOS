#include <string.h>
#include "game.h"

struct Tetris tetris_init(){
    struct Tetris t;
    t.score = 0;
    t.lines = 0;
    memset(&t.board, 0, sizeof(t.board));
}

void tetris_draw(uint32_t *buf, struct Tetris *tetris){
    const uint8_t yoff = 128 - 4*21;
    const uint8_t xoff = 4*3;
    memset(buf, 0, 128*64*sizeof(uint32_t));
    draw_outline(buf);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            if(tetris->board.b[i][j]){
                block_draw(
                    xoff+i*4,
                    yoff+j*4,
                    buf,
                    1
                );
            }
        }
    }
    invert_screen(buf);
}
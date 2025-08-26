#include <string.h>
#include "game.h"

struct Tetris tetris_init(){
    struct Tetris t;
    t.score = 0;
    t.lines = 0;
    memset(&t.board, 0, sizeof(t.board));
}

void tetris_draw(struct Viewport *vp, struct Tetris *tetris){
    
}
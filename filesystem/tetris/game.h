#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <sys/vp.h>
#include "block.h"


struct Board {
    uint8_t b[10][20];
};

struct Tetris {
    struct Board board;
    unsigned int score;
    unsigned int lines;
};

struct Tetris tetris_init();
void tetris_draw(uint32_t *buf, struct Tetris *tetris);

#endif
#ifndef BLOCK_H
#define BLOCK_H

typedef enum {
    GRAY,
    CYAN,
    PURPLE,
    YELLOW,
    RED,
    ORANGE,
    BLUE
} Bcolor;

void block_draw(int x, int y, Bcolor color, uint32_t *buf, uint32_t w, uint32_t h);

#endif
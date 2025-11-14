#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

typedef enum {
    GRAY,
    CYAN,
    PURPLE,
    YELLOW,
    RED,
    ORANGE,
    BLUE
} Bcolor;

void block_draw(uint8_t x, uint8_t y, uint32_t *buf, uint8_t mode);
void draw_outline(uint32_t *buf);
void invert_screen(uint32_t *buf);

#endif
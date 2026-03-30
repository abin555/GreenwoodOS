#ifndef FONT_H
#define FONT_H

#include <stdint.h>

int font_load();
void font_drawChar(uint32_t *buf, uint32_t width, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, char c);

#endif
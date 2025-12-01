#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>


void initScreen();

typedef enum OLED_colors{
	COLOR_BLACK, 
	COLOR_WHITE,
	COLOR_INVERT
	} OLED_color; //just to clarify down below

void writePixel(uint8_t x, uint8_t y, OLED_color color);

void invertDisplay(uint8_t inv);
void showScreen();
void clearDisplay();

#endif
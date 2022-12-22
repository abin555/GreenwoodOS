#include "image.h"

struct image_buffer_definition image_buffer_def;

void image_load_buffer(uint32_t w, uint32_t h, uint8_t* buffer){
    image_buffer_def.buffer = buffer;
    image_buffer_def.width = w;
    image_buffer_def.height = h;
}

void draw_buffer(int x, int y){
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint32_t color = 0;
    for(uint32_t scan_y = 0; scan_y < image_buffer_def.height; scan_y++){
        for(uint32_t scan_x = 0; scan_x < image_buffer_def.width; scan_x++){
            blue = image_buffer_def.buffer[4*(scan_y*image_buffer_def.width + scan_x)];
            green = image_buffer_def.buffer[4*(scan_y*image_buffer_def.width + scan_x)+1];
            red = image_buffer_def.buffer[4*(scan_y*image_buffer_def.width + scan_x)+2];
            color = 0;
            color |= red << 16;
            color |= green << 8;
            color |= blue << 0;
            fb_setPixel(x+scan_x, y+scan_y, color);
        }
    }
}
#include "icon.h"


struct Icon generateIcon(struct Bitmap bitmap, int x, int y, int w, int h, char *label){
    struct Icon new_icon = {
        bitmap,
        {
            x,
            y,
            w,
            h
        },
        {
            x,
            y,
            w,
            h
        },
        "\0\0\0\0\0",
        0
    };

    for(int i = 0; i < 5 && label[i] != '\0'; i++){
        new_icon.label[i] = label[i];
    }

    return new_icon;
}

bool getIconHover(struct Icon *icon, int x, int y){
    if(icon == NULL) return false;
    if(x > icon->loc.x && x < icon->loc.x + icon->loc.w && y > icon->loc.y && y < icon->loc.y + icon->loc.h) return true;
    return false;
}

void drawIcon(struct Icon *icon, struct WINDOW *window){
    if(icon == NULL) return;
    drawBitmap(
        icon->loc.x,
        icon->loc.y,
        icon->bitmap,
        window
    );
    if(icon->selected){
        drawRect(
            0x0000FF,
            0x0000BB,
            icon->loc.x,
            icon->loc.y,
            icon->loc.x + icon->loc.w,
            icon->loc.y + icon->loc.h,
            window->backbuffer, 
            window->width
        );
    }
    for(int i = 0; i < 5 && icon->label[i] != 0; i++){
        buf_putChar(window->backbuffer, icon->loc.x+i*8, icon->loc.y+icon->loc.h, icon->label[i], 0xFFFFFF, 0x0);
    }
}

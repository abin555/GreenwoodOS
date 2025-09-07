#ifndef ICON_H
#define ICON_H

#include <stdint.h>
#include <stdbool.h>
#include "bitmap.h"
#include <sys/window.h>
#include "gfx.h"
#include "desktop_shared.h"

struct Icon {
    struct Bitmap bitmap;
    struct Location loc;
    struct Location oldLoc;
    char label[5];
    bool selected;
};

struct Icon generateIcon(struct Bitmap bitmap, int x, int y, int w, int h, char *label);
bool getIconHover(struct Icon *icon, int x, int y);
void drawIcon(struct Icon *icon, struct WINDOW *window);

#endif
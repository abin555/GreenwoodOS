#ifndef DESKTOP_SHARED_H
#define DESKTOP_SHARED_H

#include <sys/window.h>

extern struct WINDOW *desktop_win;

struct DesktopConfig {
    int vp_root_x;
    int vp_root_y;
    int screen_w;
    int screen_h;
};

extern struct DesktopConfig desktopConfig;

struct Location {
    int x;
    int y;
    int w;
    int h;
};

#endif
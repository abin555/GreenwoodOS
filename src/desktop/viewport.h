#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "stdint.h"
#include "bitmap.h"
#include "window.h"
#include "framebuffer.h"
#include "gfx.h"
#include "desktop_shared.h"

struct Viewport;
struct Viewport {
    struct Location loc;
    struct Location oldLoc;
    uint32_t *buf;
    uint32_t buf_size;
    uint8_t resizeable;
    bool selected;
    bool minimized;
    int minimized_w;
    int minimized_h;
    char *title;

    void (*exit_event_handler)(struct Viewport *);
};

struct Viewport make_viewport(int w, int h, char *title);
void draw_viewport(struct Viewport *viewport, struct WINDOW *window);
bool getViewportHover(struct Viewport *viewport, int x, int y);
bool viewport_handle_click_event(struct Viewport *viewport, int x, int y);

#endif
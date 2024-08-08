#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "stdint.h"
#include "bitmap.h"
#include "window.h"
#include "framebuffer.h"
#include "gfx.h"
#include "desktop_shared.h"

#define VIEWPORT_HEADER_HEIGHT 10

struct Viewport;
struct Viewport {
    struct Location loc;
    struct Location oldLoc;
    uint32_t *buf;
    uint32_t buf_size;
    uint8_t resizeable;
    bool minimized;
    bool open;
    int minimized_w;
    int minimized_h;
    char *title;

    void (*exit_event_handler)(struct Viewport *);
};

#define MAX_VIEWPORTS 100

struct ViewportList_element{
    bool inUse;
    struct Viewport *vp;
};

struct ViewportList {
    struct Viewport viewports[MAX_VIEWPORTS];
    struct ViewportList_element elements[MAX_VIEWPORTS];
    int max;
    int count;
};

typedef enum {
    VP_None,
    VP_Header,
    VP_Scale,
    VP_Close,
    VP_Body
} VIEWPORT_CLICK_TYPE;

struct Viewport_Interaction{
    VIEWPORT_CLICK_TYPE clickType;
    struct Viewport *vp;
};

extern struct ViewportList *global_viewport_list;

void viewport_init_sys(struct ViewportList *viewport_list);
struct Viewport *viewport_open(struct ViewportList *viewport_list, int w, int h, char *title);
void viewport_close(struct ViewportList *viewport_list, struct Viewport *viewport);
void viewport_move_element_to_front(struct ViewportList *viewport_list, int elemIdx);
struct Viewport_Interaction viewport_process_click(struct ViewportList *viewport_list, int x, int y);
void viewport_draw_all(struct ViewportList *viewport_list, struct WINDOW *window);

struct Viewport make_viewport(int w, int h, char *title);
void draw_viewport(struct Viewport *viewport, struct WINDOW *window);
void viewport_draw_buf(struct Viewport *viewport, struct WINDOW *window);
bool getViewportTitleClick(struct Viewport *viewport, int x, int y);
bool getViewportBodyClick(struct Viewport *viewport, int x, int y);
VIEWPORT_CLICK_TYPE viewport_handle_title_click_event(struct Viewport *viewport, int x, int y);
void viewport_set_position(struct Viewport *viewport, struct WINDOW *window, int x, int y);
void viewport_set_buffer(struct Viewport *viewport, uint32_t *buffer, uint32_t buf_size);

#endif
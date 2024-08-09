#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "stdint.h"
#include "gfx.h"
#include "desktop_shared.h"

#define VIEWPORT_HEADER_HEIGHT 10

typedef enum {
    VP_FOCUSED,
    VP_UNFOCUSED,
    VP_KEY,
    VP_EXIT,
    VP_MINIMIZE,
    VP_MAXIMIZE
} VIEWPORT_EVENT_TYPE;

struct Viewport {
    struct Location loc;
    struct Location oldLoc;
    uint32_t *frontbuf;
    uint32_t *backbuf;
    uint32_t buf_size;
    uint8_t resizeable;
    bool minimized;
    bool open;
    int minimized_w;
    int minimized_h;
    char *title;
    int owner_program_slot;
    int owner_task_id;
    char ascii;

    void (*event_handler)(struct Viewport *, VIEWPORT_EVENT_TYPE);
};

#define MAX_VIEWPORTS 100

struct ViewportList_element{
    bool inUse;
    struct Viewport *vp;
};

struct ViewportList {
    struct Viewport viewports[MAX_VIEWPORTS];
    struct ViewportList_element elements[MAX_VIEWPORTS];
    uint32_t *frontbuf_region;
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

struct ViewportFunctions {
    struct Viewport *(*open)(int, int, char*);
    void (*close)(struct Viewport*);
    void (*set_buffer)(struct Viewport*, uint32_t*, uint32_t);
    void (*copy)(struct Viewport*);
    void (*add_event_handler)(struct Viewport *, void (*)(struct Viewport *, VIEWPORT_EVENT_TYPE));
    void (*drawChar)(struct Viewport *, int, int, char, uint32_t, uint32_t);
    char (*getc)(struct Viewport *);
};

extern struct ViewportFunctions global_viewport_functions;
extern struct ViewportList *global_viewport_list;


void vp_draw_char(struct Viewport *vp, int x, int y, char c, uint32_t fg, uint32_t bg);
char viewport_getc(struct Viewport *vp);
void viewport_init_sys(struct ViewportList *viewport_list);
struct Viewport *viewport_indirect_open(int w, int h, char *title);
struct Viewport *viewport_open(struct ViewportList *viewport_list, int w, int h, char *title);
void viewport_indirect_close(struct Viewport *viewport);
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
void viewport_copy_buffer(struct Viewport *viewport);
void viewport_add_event_handler(struct Viewport *viewport, void (*handler)(struct Viewport *viewport, VIEWPORT_EVENT_TYPE));
void viewport_send_event(struct Viewport *viewport, VIEWPORT_EVENT_TYPE event);

#endif
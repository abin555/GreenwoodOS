#ifndef SYS_VP_H
#define SYS_VP_H
#include <stdint.h>

struct Location {
    int x;
    int y;
    int w;
    int h;
};

typedef enum {
    VP_FOCUSED,
    VP_UNFOCUSED,
    VP_KEY,
    VP_EXIT,
    VP_MINIMIZE,
    VP_MAXIMIZE,
    VP_CLICK
} VIEWPORT_EVENT_TYPE;

struct Viewport {
    struct Location loc;
    struct Location oldLoc;
    uint32_t *frontbuf;
    uint32_t *backbuf;
    uint32_t buf_size;
    uint8_t resizeable;
    char minimized;
    char open;
    int minimized_w;
    int minimized_h;
    char *title;
    int owner_program_slot;
    int owner_task_id;
    char ascii;

    void (*event_handler)(struct Viewport *, VIEWPORT_EVENT_TYPE);
	char click_events_enabled;
    char transparent;
};

struct Viewport *vp_open(int w, int h, char *title);
void vp_close(struct Viewport *vp);
void vp_set_buffer(struct Viewport *vp, uint32_t *buf, uint32_t size);
void vp_copy(struct Viewport *vp);
void vp_add_event_handler(struct Viewport *vp, void (*)(struct Viewport *, VIEWPORT_EVENT_TYPE));
void vp_drawChar(struct Viewport *vp, int x, int y, char c, uint32_t fg, uint32_t bg);
char vp_getc(struct Viewport *vp);

#endif
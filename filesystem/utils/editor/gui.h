#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <sys/vp.h>
#include <sys/mouse.h>


extern struct ViewportFunctions *vp_funcs;

typedef enum {
    GUI_BUTTON,
    GUI_BAR,
    GUI_BOX,
    GUI_CONTEXT,
    GUI_SCROLLV,
    GUI_SCROLLH
} GUI_ELEMENT;

struct GUIElement {
    GUI_ELEMENT type;
    uint32_t tag_size;
};

struct GUIChildren {
    struct GUIElement **children;
    int numChildren;
    int maxChildren;
};

struct GUIButton {
    GUI_ELEMENT type;
    uint32_t tag_size;

    uint32_t textColor;
    uint32_t backgroundColor;

    uint32_t hoverTextColor;
    uint32_t hoverBGColor;
    char hasHover;

    struct Location location;
    char *text;

    char isClicked;
    char clickLocked;
    char isHovered;
};

struct GUIBar {
    GUI_ELEMENT type;
    uint32_t tag_size;

    uint32_t innerColor;
    uint32_t outerColor;
    struct Location location;
    struct GUIChildren children;
};

struct GUIScroll {
    GUI_ELEMENT type;
    uint32_t tag_size;
    struct Location location;

    int barSize;
    uint32_t innerColor;
    uint32_t outerColor;
    uint32_t handleColor;
    double scroll;

    char isClicked;
    char clickLocked;
    char isHovered;
    struct {
        int x;
        int y;
    } mouseStart;
};

struct WindowContext {
    GUI_ELEMENT type;
    uint32_t tag_size;

    struct Viewport *viewport;
    uint32_t *buffer;
    uint32_t buffer_size;
    uint32_t width;
    uint32_t height;

    uint32_t backgroundColor;

    struct GUIChildren children;
    struct GUIElement *rightClick;
    struct MouseStatus mouseStatus;

    int localMouseX;
    int localMouseY;
};

void gui_setup();

struct WindowContext *gui_makeContext(char *text, int width, int height, int maxChildren, uint32_t background);
void gui_closeContext(struct WindowContext *context);
void gui_addChild(void *e, void *c);
struct GUIButton *gui_makeButton(char *text, uint32_t tColor, uint32_t bgColor);
struct GUIBar *gui_makeBar(int w, int h, uint32_t iColor, uint32_t oColor, int maxChildren);
struct GUIScroll *gui_makeVScroll(int w, int h, int barSize, uint32_t iColor, uint32_t oColor, uint32_t hColor);
struct GUIScroll *gui_makeHScroll(int w, int h, int barSize, uint32_t iColor, uint32_t oColor, uint32_t hColor);

void gui_setLocation(void *e, int x, int y);
void gui_setScale(void *e, int w, int h);
void gui_buttonSetHover(struct GUIButton *button, uint32_t textColor, uint32_t backgroundColor);

void gui_handleContext(struct WindowContext *context);


void gui_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

void drawRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
);

void fillRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width,
    uint32_t buf_size
);

#endif
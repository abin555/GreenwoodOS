#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "libc.h"

struct Vec2 {
    int x;
    int y;
};

class Window{
    private:
    public:

    struct WINDOW *window;
    struct Viewport *vp;
    Window(char *name);
    Window();
    void close();
    void update();
    void clear(uint32_t color);
    int getWidth();
    int getHeight();
    uint32_t *getBuffer();
    struct Vec2 getRelativeMouse();
};

class System{
    private:
    void *memory_region;
    void *nextAlloc;

    public:
    Window window;
    struct ViewportFunctions *vp_funcs;
    int running;
    void setup();
    void *malloc(unsigned int size);
    void cleanup();
};

extern System sys;
extern struct MouseStatus *mouse;

#endif
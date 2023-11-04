#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "libc.h"

class Window{
    private:
    public:

    struct WINDOW *window;
    Window(char *name);
    Window();
    void close();
    void update();
    void clear(uint32_t color);
    int getWidth();
    int getHeight();
    uint32_t *getBuffer();
};

class System{
    private:
    void *memory_region;
    void *nextAlloc;

    public:
    Window window;
    void setup();
    void *malloc(unsigned int size);
};

extern System sys;

#endif
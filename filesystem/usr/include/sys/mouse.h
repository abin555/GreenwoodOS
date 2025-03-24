#ifndef SYS_MOUSE_H
#define SYS_MOUSE_H

struct IVec2{
    int x;
    int y;
};

struct MouseButtons{
    char right : 1;
    char left : 1;
    char middle : 1;
};

struct MouseStatus{
    struct IVec2 pos;
    struct MouseButtons buttons;
    struct IVec2 lastDelta;
};

#endif
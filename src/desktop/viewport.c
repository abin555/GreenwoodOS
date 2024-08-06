#include "viewport.h"

struct Viewport make_viewport(int w, int h, char *title){
    struct Viewport viewport;
    viewport.loc.x = 50;
    viewport.loc.y = 50;
    viewport.loc.w = w;
    viewport.loc.h = h;
    viewport.resizeable = 1;
    viewport.selected = 0;
    viewport.minimized = 0;
    viewport.title = title;
    viewport.minimized_w = viewport.loc.w;
    viewport.minimized_h = viewport.loc.h;
    return viewport;
}

void draw_viewport(struct Viewport *viewport, struct WINDOW *window){
    if(viewport == NULL || window == NULL) return;
    if(viewport->minimized){
        fillRect(
            0x0,
            0xBBBBBB,
            viewport->loc.x,
            viewport->loc.y,
            viewport->loc.x + viewport->loc.w,
            viewport->loc.y + 10,
            window->backbuffer,
            window->width
        );
        buf_putChar(
            window->backbuffer,
            viewport->loc.x + viewport->loc.w - 16,
            viewport->loc.y + 1,
            '+',
            0xFFFFFF,
            0xFF
        );
        buf_putChar(
            window->backbuffer,
            viewport->loc.x + viewport->loc.w - 8,
            viewport->loc.y + 1,
            'X',
            0xFFFFFF,
            0xFF0000
        );
    }
    else{
        fillRect(
            0x0,
            0xBBBBBB,
            viewport->loc.x,
            viewport->loc.y,
            viewport->loc.x + viewport->loc.w,
            viewport->loc.y + 10,
            window->backbuffer,
            window->width
        );
        fillRect(
            0x0,
            0x222222,
            viewport->loc.x,
            viewport->loc.y+10,
            viewport->loc.x + viewport->loc.w,
            viewport->loc.y + viewport->loc.h,
            window->backbuffer,
            window->width
        );
        buf_putChar(
            window->backbuffer,
            viewport->loc.x + viewport->loc.w - 16,
            viewport->loc.y + 1,
            '-',
            0xFFFFFF,
            0xFF
        );
        buf_putChar(
            window->backbuffer,
            viewport->loc.x + viewport->loc.w - 8,
            viewport->loc.y + 1,
            'X',
            0xFFFFFF,
            0xFF0000
        );
    }

    for(int i = 0; viewport->title[i] != '\0' && viewport->title != NULL; i++){
        buf_putChar(
            window->backbuffer,
            viewport->loc.x+i*8+2,
            viewport->loc.y+2,
            viewport->title[i],
            0x0,
            0xBBBBBB
        );
    }
}

bool viewport_handle_click_event(struct Viewport *viewport, int x, int y){
    if(!getViewportHover(viewport, x, y)) return false;
    if(x > viewport->loc.x + viewport->loc.w - 16 && x < viewport->loc.x + viewport->loc.w - 8){
        viewport->minimized = !viewport->minimized;
        if(viewport->minimized){
            viewport->minimized_w = viewport->loc.w;
            viewport->minimized_h = viewport->loc.h;
            int title_len = 0;
            char *title_counter = viewport->title;
            while(title_counter[title_len] && title_len < 50){
                title_len++;
            }
            viewport->loc.w = (8*title_len) + 3*8;
            viewport->loc.h = 8;
        }
        else{
            viewport->loc.w = viewport->minimized_w;
            viewport->loc.h = viewport->minimized_h;
        }
        return false;
    }
    else if(x > viewport->loc.x + viewport->loc.w - 8 && x < viewport->loc.x + viewport->loc.w){
        return false;
    }
    return true;
}

bool getViewportHover(struct Viewport *viewport, int x, int y){
    if(viewport == NULL) return false;
    if(x > viewport->loc.x && x < viewport->loc.x + viewport->loc.w && y > viewport->loc.y && y < viewport->loc.y + 10) return true;
    return false;
}
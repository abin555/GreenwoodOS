#include "viewport.h"
#include "framebuffer.h"
#include "bitmap.h"
#include "window.h"
#include "multitasking.h"

struct Viewport make_viewport(int w, int h, char *title){
    struct Viewport viewport;
    viewport.loc.x = 50;
    viewport.loc.y = 50;
    viewport.loc.w = w;
    viewport.loc.h = h;
    viewport.resizeable = 1;
    viewport.minimized = 0;
    viewport.open = true;
    viewport.title = title;
    viewport.minimized_w = viewport.loc.w;
    viewport.minimized_h = viewport.loc.h;
    viewport.backbuf = NULL;
    viewport.frontbuf = NULL;
    viewport.buf_size = 0;
    viewport.owner_program_slot = tasks[task_running_idx].program_slot;
    viewport.owner_task_id = task_running_idx;
    viewport.event_handler = NULL;
    viewport.ascii = '\0';
    viewport.click_events_enabled = false;
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
            viewport->loc.y + VIEWPORT_HEADER_HEIGHT,
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
            viewport->loc.y + VIEWPORT_HEADER_HEIGHT,
            window->backbuffer,
            window->width
        );
        if(viewport->backbuf == NULL || viewport->buf_size == 0){
            fillRect(
                0x0,
                0x222222,
                viewport->loc.x,
                viewport->loc.y+10,
                viewport->loc.x + viewport->loc.w,
                viewport->loc.y + viewport->loc.h + VIEWPORT_HEADER_HEIGHT,
                window->backbuffer,
                window->width
            );
        }
        else{
            viewport_draw_buf(viewport, window);
        }

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

void viewport_toggle_size(struct Viewport *viewport){
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
        viewport_send_event(viewport, VP_MINIMIZE);
    }
    else{
        viewport->loc.w = viewport->minimized_w;
        viewport->loc.h = viewport->minimized_h;
        viewport_send_event(viewport, VP_MAXIMIZE);
    }
}

VIEWPORT_CLICK_TYPE viewport_handle_title_click_event(struct Viewport *viewport, int x, int y){
    if(!getViewportTitleClick(viewport, x, y)) return VP_None;
    if(x > viewport->loc.x + viewport->loc.w - 16 && x < viewport->loc.x + viewport->loc.w - 8){
        viewport_toggle_size(viewport);
        return VP_Scale;
    }
    else if(x > viewport->loc.x + viewport->loc.w - 8 && x < viewport->loc.x + viewport->loc.w){
        viewport_send_event(viewport, VP_EXIT);
        return VP_Close;
    } 
    return VP_None;
}

__attribute__((section(".text")))
struct ViewportList *global_viewport_list;

__attribute__((section(".text")))
struct ViewportFunctions global_viewport_functions = {
    viewport_indirect_open,
    viewport_indirect_close,
    viewport_set_buffer,
    viewport_copy_buffer,
    viewport_add_event_handler,
    vp_draw_char,
    viewport_getc
};

char viewport_getc(struct Viewport *vp){
    if(vp == NULL) return '\0';
    while(vp->ascii == '\0'){}
    char c = vp->ascii;
    vp->ascii = '\0';
    //print_serial("[VIEWPORT] %s getc call returns %c\n", vp->title, c);
    return c;
}

void vp_draw_char(struct Viewport *vp, int x, int y, char c, uint32_t fg, uint32_t bg){
    if(vp == NULL || vp->backbuf == NULL) return;
    buf_w_putChar(
        vp->backbuf,
        vp->loc.w,
        x, y,
        c,
        fg, bg
    );
}

bool getViewportTitleClick(struct Viewport *viewport, int x, int y){
    if(viewport == NULL) return false;
    if(x > viewport->loc.x && x < viewport->loc.x + viewport->loc.w && y > viewport->loc.y && y < viewport->loc.y + VIEWPORT_HEADER_HEIGHT) return true;
    return false;
}

void viewport_init_sys(struct ViewportList *viewport_list){
    if(viewport_list == NULL){
        print_serial("[VIEWPORT] List is null!\n");
    }
    viewport_list->max = MAX_VIEWPORTS;
    viewport_list->count = 0;

    print_serial("[VIEWPORT] Init System %x %d %d\n", viewport_list, viewport_list->max, viewport_list->count);

    int block = MEM_findRegionIdx(MAX_VIEWPORTS * fb_width * fb_height * sizeof(uint32_t));
    uint32_t addr = MEM_reserveRegionBlock(block, MAX_VIEWPORTS * fb_width * fb_height * sizeof(uint32_t), 0, FRAMEBUFFER);

    viewport_list->frontbuf_region = (uint32_t *) addr;
    MEM_printRegions();

    for(int i = 0; i < viewport_list->max; i++){
        viewport_list->viewports[i].open = false;
        viewport_list->elements[i].inUse = false;
    } 

}

struct Viewport *viewport_indirect_open(int w, int h, char *title){
    int title_len = 0;
    char *title_ptr = title;
    while(*title_ptr++) title_len++;
    title_len++;
    title_ptr = malloc(title_len);
    for(int i = 0; i < title_len; i++) title_ptr[i] = title[i];
    return viewport_open(global_viewport_list, w, h, title_ptr);
}

struct Viewport *__attribute__ ((optimize("-O3"))) viewport_open(struct ViewportList *viewport_list, int w, int h, char *title){
    print_serial("[VIEWPORT] Open Window W: %d H: %d Title: %s\n", w, h, title);
    if(w > (int) fb_width || h > (int) fb_height || w < 0 || h < 0) return NULL;
    int element_idx = -1;
    int viewport_idx = -1;
    for(int i = 0; i < viewport_list->max; i++){
        if(viewport_list->elements[i].inUse == false){
            print_serial("[VIEWPORT] Element %d is not in use\n", i);
            element_idx = i;
            viewport_list->count++;
            break;
        }
    }
    if(element_idx == -1) return NULL;
    for(int i = 0; i < viewport_list->max; i++){
        if(!viewport_list->viewports[i].open){
            viewport_idx = i;
            break;
        }
    }
    viewport_list->elements[element_idx].inUse = true;
    viewport_list->elements[element_idx].vp = &viewport_list->viewports[viewport_idx];
    
    viewport_list->viewports[viewport_idx] = make_viewport(w, h, title);
    print_serial("[VIEWPORT] Opened - Elem: %d - VP: %d & %x Count: %d\n", element_idx, viewport_idx, viewport_list->elements[element_idx].vp, viewport_list->count);
    viewport_move_element_to_front(viewport_list, element_idx);
    return &viewport_list->viewports[viewport_idx];
}

void viewport_indirect_close(struct Viewport *viewport){
    viewport_close(global_viewport_list, viewport);
}

void __attribute__ ((optimize("-O3"))) viewport_close(struct ViewportList *viewport_list, struct Viewport *viewport){
    if(viewport == NULL) return;
    viewport->open = false;
    viewport->event_handler = NULL;
    int drop_idx = -1;
    for(int i = 0; i < viewport_list->count; i++){
        if(viewport_list->elements[i].vp == viewport){
            viewport_list->elements[i].vp = NULL;
            viewport_list->elements[i].inUse = false;
            drop_idx = i;
            viewport_list->count--;
            break;
        }
    }
    if(drop_idx == -1) return;
    for(int i = drop_idx; i < viewport_list->count; i++){
        struct ViewportList_element temp = viewport_list->elements[i+1];
        viewport_list->elements[i+1] = viewport_list->elements[i];
        viewport_list->elements[i] = temp;
    }
    viewport_send_event(viewport_list->elements[0].vp, VP_FOCUSED);
}

void __attribute__ ((optimize("-O3"))) viewport_move_element_to_front(struct ViewportList *viewport_list, int element_idx){
    if(element_idx == 0) return;
    for(int i = element_idx; i > 0; i--){
        struct ViewportList_element temp = viewport_list->elements[i - 1];
        viewport_list->elements[i - 1] = viewport_list->elements[i];
        viewport_list->elements[i] = temp;
    }
    viewport_send_event(viewport_list->elements[0].vp, VP_FOCUSED);
    viewport_send_event(viewport_list->elements[1].vp, VP_UNFOCUSED);
    return;
}

void __attribute__ ((optimize("-O3"))) viewport_draw_all(struct ViewportList *viewport_list, struct WINDOW *window){
    for(int i = viewport_list->count - 1; i >= 0; i--){
        if(!viewport_list->elements[i].inUse){
            //print_serial("[VIEWPORT] %d not in use\n", i);
            continue;
        }
        if(viewport_list->elements[i].vp == NULL){
            //print_serial("[VIEWPORT] %d is NULL\n", i);
            continue;
        }
        if(!viewport_list->elements[i].vp->open){
            //print_serial("[VIEWPORT] %d is not open\n", i);
            continue;
        }
        draw_viewport(viewport_list->elements[i].vp, window);
    }
}

struct Viewport_Interaction __attribute__ ((optimize("-O3"))) viewport_process_click(struct ViewportList *viewport_list, int x, int y){
    struct Viewport_Interaction interaction = {
        VP_None,
        NULL
    };
    for(int i = 0; i < viewport_list->count; i++){
        if(!viewport_list->elements[i].inUse) continue;
        if(viewport_list->elements[i].vp == NULL) continue;
        struct Viewport *vp = viewport_list->elements[i].vp;
        if(getViewportTitleClick(vp, x, y)){
            interaction.clickType = VP_Header;
            interaction.vp = vp;
            viewport_move_element_to_front(viewport_list, i);
            VIEWPORT_CLICK_TYPE button_check = viewport_handle_title_click_event(vp, x, y);
            if(button_check != VP_None){
                interaction.clickType = button_check;
            }
            return interaction;
        }
        else if(getViewportBodyClick(vp, x, y)){
            interaction.clickType = VP_Body;
            interaction.vp = vp;
            viewport_move_element_to_front(viewport_list, i);
            if(vp->click_events_enabled) viewport_send_event(vp, VP_CLICK);
            return interaction;
        }
    }
    return interaction;
}

void __attribute__ ((optimize("-O3"))) viewport_set_position(struct Viewport *viewport, struct WINDOW *window, int x, int y){
    if(viewport == NULL) return;
    viewport->loc.x = x;
    viewport->loc.y = y;

    if(viewport->loc.x < 0) viewport->loc.x = 0;
    if(viewport->loc.y < 0) viewport->loc.y = 0;
    if(viewport->loc.x + viewport->loc.w >= (int) window->width) viewport->loc.x = window->width - viewport->loc.w;
    if(viewport->loc.y + viewport->loc.h >= (int) window->height) viewport->loc.y = window->height - viewport->loc.h;
}

bool __attribute__ ((optimize("-O3"))) getViewportBodyClick(struct Viewport *viewport, int x, int y){
    if(viewport == NULL) return false;
    if(x > viewport->loc.x && x < viewport->loc.x + viewport->loc.w && y > viewport->loc.y + VIEWPORT_HEADER_HEIGHT && y < viewport->loc.y + viewport->loc.h + VIEWPORT_HEADER_HEIGHT) return true;
    return false;
}

void viewport_set_buffer(struct Viewport *viewport, uint32_t *buffer, uint32_t buf_size){
    if(viewport == NULL || buffer == NULL) return;
    viewport->backbuf = buffer;
    viewport->buf_size = buf_size;
    int frontbuf_idx = viewport - global_viewport_list->viewports;
    viewport->frontbuf = global_viewport_list->frontbuf_region + (fb_width * fb_height * frontbuf_idx);
}

void __attribute__ ((optimize("-O3"))) viewport_draw_buf(struct Viewport *viewport, struct WINDOW *window){
    uint32_t x = (uint32_t) viewport->loc.x;
    uint32_t y = (uint32_t) viewport->loc.y + VIEWPORT_HEADER_HEIGHT;
    uint32_t w = (uint32_t) viewport->loc.w;
    uint32_t h = (uint32_t) viewport->loc.h;

    uint32_t wbytes = sizeof(uint32_t) * w;
    uint32_t wy = y*window->width;

    for(uint32_t ly = 0; ly < h; ly++){
        uint32_t yoff = (ly)*window->width;
        //uint32_t wyoff = yoff + wy;
        memfcpy(window->backbuffer + wy + yoff + x, viewport->frontbuf + ly*w, wbytes);
        /*
        for(int lx = 0; lx < (int) w; lx++){
            window->backbuffer[(y+ly)*window->width + (x+lx)] = viewport->frontbuf[ly*w+lx];
        }
        */
    }
    return;
}

void viewport_copy_buffer(struct Viewport *viewport){
    if(viewport == NULL) return;
    if(viewport->backbuf == NULL || viewport->frontbuf == NULL || viewport->minimized || !viewport->open) return;
    memfcpy(viewport->frontbuf, viewport->backbuf, viewport->buf_size);
}

void viewport_add_event_handler(struct Viewport *viewport, void (*handler)(struct Viewport *, VIEWPORT_EVENT_TYPE)){
    if(viewport == NULL || handler == NULL) return;
    print_serial("[VIEWPORT] %s (SLOT %d) added event handler 0x%x\n", viewport->title, viewport->owner_program_slot, (uint32_t) handler);
    viewport->event_handler = handler;
}

void viewport_send_event(struct Viewport *viewport, VIEWPORT_EVENT_TYPE event){
    if(viewport == NULL || viewport->event_handler == NULL) return;
    //print_serial("[VIEWPORT] Sending event %d to viewport %s (SLOT %d) @ 0x%x\n", event, viewport->title, viewport->owner_program_slot, viewport->event_handler);
    task_lock = 1;
    int current_task_id = task_running_idx;

    if(viewport->owner_program_slot != -1){
        task_running_idx = viewport->owner_task_id;
        select_program(viewport->owner_program_slot);
    }
    
    viewport->event_handler(viewport, event);
    
    task_running_idx = current_task_id;
    if(tasks[task_running_idx].program_slot != -1){
        select_program(tasks[task_running_idx].program_slot);
    }
    task_lock = 0;
    //print_serial("[VIEWPORT] Sent event\n");
}
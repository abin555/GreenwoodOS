#include <stdlib.h>
#include <stdio.h>
#include <sys/io.h>
#include <sys/task.h>
#include "gui.h"

struct ViewportFunctions *vp_funcs;
int mouse_fd;

void gui_setup(){
    mouse_fd = open("/-/sys/mouse", O_READ);
    if(mouse_fd == -1){
        printf("Error, unable to open mouse file!\n");
        exit(1);
    }
}

struct WindowContext *gui_makeContext(char *text, int width, int height, int maxChildren, uint32_t background){
    struct WindowContext *context = malloc(sizeof(struct WindowContext));
    if(context == NULL) return NULL;
    context->type = GUI_CONTEXT;
    context->tag_size = sizeof(struct WindowContext);

    context->buffer_size = sizeof(uint32_t) * width * height;
    context->viewport = vp_open(width, height, text);
    context->buffer = malloc(context->buffer_size);

    vp_add_event_handler(context->viewport, gui_event);
    vp_set_buffer(context->viewport, context->buffer, context->buffer_size);

    context->width = width;
    context->height = height;

    context->children.children = malloc(sizeof(struct GUIElement *) * maxChildren);
    context->children.maxChildren = maxChildren;
    context->children.numChildren = 0;

    context->backgroundColor = background;

    return context;
}

void gui_closeContext(struct WindowContext *context){
    close(mouse_fd);
    vp_close(context->viewport);
}

void gui_addChild(void *e, void *c){
    struct GUIElement *elem = e;
    struct GUIElement *child = c;
    if(elem == NULL || child == NULL) return;

    if(elem->type == GUI_CONTEXT){
        struct WindowContext *context = (struct WindowContext *) elem;
        if(context->children.numChildren == context->children.maxChildren) return;
        context->children.children[context->children.numChildren] = child;
        context->children.numChildren++;
    }
    else if(elem->type == GUI_BAR){
        struct GUIBar *bar = (struct GUIBar *) elem;
        if(bar->children.numChildren == bar->children.maxChildren) return;
        bar->children.children[bar->children.numChildren] = child;
        bar->children.numChildren++;
    }
    else if(elem->type == GUI_BUTTON){
        struct GUIButton *button = (struct GUIButton *) elem;
        return;
    }
    else if(elem->type == GUI_SCROLL){
        struct GUIScroll *scroll = (struct GUIScroll *) elem;
        return;
    }
}

void gui_setLocation(void *e, int x, int y){
    struct GUIElement *elem = e;
    if(elem == NULL) return;

    if(elem->type == GUI_CONTEXT){
        struct WindowContext *context = (struct WindowContext *) elem;
        context->viewport->loc.x = x;
        context->viewport->loc.y = y;
    }
    else if(elem->type == GUI_BAR){
        struct GUIBar *bar = (struct GUIBar *) elem;
        bar->location.x = x;
        bar->location.y = y;
    }
    else if(elem->type == GUI_BUTTON){
        struct GUIButton *button = (struct GUIButton *) elem;
        button->location.x = x;
        button->location.y = y;
    }
    else if(elem->type == GUI_SCROLL){
        struct GUIScroll *scroll = (struct GUIScroll *) elem;
        scroll->location.x = x;
        scroll->location.y = y;
    }
}

void gui_setScale(void *e, int w, int h){
    struct GUIElement *elem = e;
    if(elem->type == GUI_CONTEXT){
        struct WindowContext *context = (struct WindowContext *) elem;
        return;
    }
    else if(elem->type == GUI_BAR){
        struct GUIBar *bar = (struct GUIBar *) elem;
        bar->location.w = w;
        bar->location.h = h;
    }
    else if(elem->type == GUI_BUTTON){
        struct GUIButton *button = (struct GUIButton *) elem;
        button->location.w = w;
        button->location.h = h;
    }
    else if(elem->type == GUI_SCROLL){
        struct GUIScroll *scroll = (struct GUIScroll *) elem;
        scroll->location.w = w;
        scroll->location.h = h;
    }
}

struct GUIButton *gui_makeButton(char *text, uint32_t tColor, uint32_t bgColor){
    struct GUIButton *button = malloc(sizeof(struct GUIButton));
    if(button == NULL) return NULL;
    button->type = GUI_BUTTON;
    button->tag_size = sizeof(struct GUIButton);

    button->textColor = tColor;
    button->backgroundColor = bgColor;

    button->location.x = 0;
    button->location.y = 0;
    button->location.w = 0;
    button->location.h = 0;

    button->isClicked = 0;
    button->isHovered = 0;
    button->hasHover = 0;
    button->text = text;
    printf("[GUI] Made Button\n");
    return button;
}

struct GUIBar *gui_makeBar(int w, int h, uint32_t iColor, uint32_t oColor, int maxChildren){
    struct GUIBar *bar = malloc(sizeof(struct GUIBar));
    if(bar == NULL) return NULL;

    bar->type = GUI_BAR;
    bar->tag_size = sizeof(struct GUIBar);

    bar->innerColor = iColor;
    bar->outerColor = oColor;

    bar->children.children = malloc(sizeof(struct GUIElement *) * maxChildren);
    bar->children.maxChildren = maxChildren;
    bar->children.numChildren = 0;

    bar->location.w = w;
    bar->location.h = h;

    return bar;
}

struct GUIScroll *gui_makeScroll(int w, int h, int barHeight, uint32_t iColor, uint32_t oColor, uint32_t hColor){
    struct GUIScroll *scroll = malloc(sizeof(struct GUIScroll));
    if(scroll == NULL) return NULL;

    scroll->type = GUI_SCROLL;
    scroll->tag_size = sizeof(struct GUIScroll);

    scroll->location.x = 0;
    scroll->location.y = 0;
    scroll->location.w = w;
    scroll->location.h = h;

    scroll->barHeight = barHeight;
    scroll->innerColor = iColor;
    scroll->outerColor = oColor;
    scroll->handleColor = hColor;

    scroll->scroll = 0.0f;
    printf("[GUI] Made Scroll\n");
    return scroll;
}

void gui_buttonSetHover(struct GUIButton *button, uint32_t textColor, uint32_t backgroundColor){
    if(button == NULL) return;
    button->hoverTextColor = textColor;
    button->hoverBGColor = backgroundColor;
    button->hasHover = 1;
}

void gui_drawElement(struct WindowContext *context, struct GUIElement *elem, int x, int y){
    if(context == NULL || elem == NULL) return;
    if(elem->type == GUI_CONTEXT){
        struct WindowContext *elemContext = (struct WindowContext *) elem;
        for(int i = 0; i < elemContext->width * elemContext->height; i++){
            elemContext->buffer[i] = elemContext->backgroundColor;
        }
        for(int i = 0; i < elemContext->children.numChildren; i++){
            gui_drawElement(context, elemContext->children.children[i], 0, 0);
        }
    }
    else if(elem->type == GUI_BAR){
        struct GUIBar *bar = (struct GUIBar *) elem;
        fillRect(
            bar->innerColor,
            bar->outerColor,
            bar->location.x + x,
            bar->location.y + y,
            bar->location.x + x + bar->location.w,
            bar->location.y + y + bar->location.h,
            context->buffer,
            context->width
        );
        for(int i = 0; i < bar->children.numChildren; i++){
            gui_drawElement(context, bar->children.children[i], bar->location.x + x + 1, bar->location.y + y + 1);
        }
    }
    else if(elem->type == GUI_BUTTON){
        struct GUIButton *button = (struct GUIButton *) elem;
        uint32_t bg_color = button->backgroundColor;
        uint32_t fg_color = button->textColor;
        if(button->isHovered && button->hasHover){
            bg_color = button->hoverBGColor;
            fg_color = button->hoverTextColor;
        }
        fillRect(
            bg_color,
            bg_color,
            button->location.x + x,
            button->location.y + y,
            button->location.x + x + button->location.w,
            button->location.y + y + button->location.h,
            context->buffer,
            context->width
        );
        if(button->text != NULL){
            for(int i = 0; button->text[i] != '\0'; i++){
                vp_drawChar(
                    context->viewport,
                    button->location.x + x + i*8,
                    button->location.y + y,
                    button->text[i],
                    fg_color,
                    bg_color
                );
            }
        }
    }
    else if(elem->type == GUI_SCROLL){
        struct GUIScroll *scroll = (struct GUIScroll *) elem;
        //printf("Draw Scroll\n");
        
        fillRect(
            scroll->innerColor,
            scroll->outerColor,
            scroll->location.x + x,
            scroll->location.y + y,
            scroll->location.x + x + scroll->location.w,
            scroll->location.y + y + scroll->location.h,
            context->buffer,
            context->width
        );
        
        
        int scrollSpaceSize = scroll->location.h - scroll->barHeight;
        int barY = (int) (scrollSpaceSize * scroll->scroll);
        fillRect(
            scroll->handleColor,
            scroll->outerColor,
            scroll->location.x + x,
            scroll->location.y + y + barY,
            scroll->location.x + x + scroll->location.w,
            scroll->location.y + y + barY + scroll->barHeight,
            context->buffer,
            context->width
        );
        
        //printf("Finish Draw Scroll\n");
    }
}

void gui_processInteraction(int mouseX, int mouseY, struct WindowContext *context, struct GUIElement *elem, int x, int y){
    if(context == NULL || elem == NULL){
        printf("Interaction Fail, null input\n");
    }
    //printf("Process Interaction\n");
    if(elem->type == GUI_CONTEXT){
        //printf("Context\n");
        struct WindowContext *elemContext = (struct WindowContext *) elem;
        for(int i = 0; i < elemContext->children.numChildren; i++){
            gui_processInteraction(mouseX, mouseY, context, elemContext->children.children[i], 0, 0);
        }
    }
    else if(elem->type == GUI_BAR){
        //printf("Bar\n");
        struct GUIBar *bar = (struct GUIBar *) elem;
        
        for(int i = 0; i < bar->children.numChildren; i++){
            gui_processInteraction(mouseX, mouseY, context, bar->children.children[i], bar->location.x + x + 1, bar->location.y + y + 1);
        }
    }
    else if(elem->type == GUI_BUTTON){
        //printf("Button\n");
        struct GUIButton *button = (struct GUIButton *) elem;
        if(
            mouseX > button->location.x + x && mouseX < button->location.x + x + button->location.w &&
            mouseY > button->location.y + y && mouseY < button->location.y + y + button->location.h
        ){
            button->isHovered = 1;
        }
        else{
            button->isHovered = 0;
        }
        if(button->isHovered && !button->clickLocked && context->mouseStatus.buttons.left){
            button->isClicked = 1;
            button->clickLocked = 1;
        }
        if(!context->mouseStatus.buttons.left){
            button->clickLocked = 0;
        }
    }
    else if(elem->type == GUI_SCROLL){
        struct GUIScroll *scroll = (struct GUIScroll *) elem;
        if(
            mouseX > scroll->location.x + x && mouseX < scroll->location.x + x + scroll->location.w &&
            mouseY > scroll->location.y + y && mouseY < scroll->location.y + y + scroll->location.h
        ){
            scroll->isHovered = 1;
        }
        else{
            scroll->isHovered = 0;
        }
        if(scroll->isHovered && !scroll->clickLocked && context->mouseStatus.buttons.left){
            scroll->isClicked = 1;
            scroll->clickLocked = 1;
            scroll->mouseStart.x = mouseX;
            scroll->mouseStart.y = mouseY;
        }
        if(!context->mouseStatus.buttons.left){
            scroll->clickLocked = 0;
        }
        if(scroll->clickLocked){
            int deltaY = mouseY - scroll->location.y;
            double proportion = (double) ((double)deltaY / (double)scroll->location.h);
            if(!(proportion < 0 || proportion > 1)){
                scroll->scroll = proportion;
            }
        }
        return;
    }
    //printf("Finish process interaction\n");
}

void gui_handleContext(struct WindowContext *context){
    read(mouse_fd, &context->mouseStatus, sizeof(context->mouseStatus));
    lseek(mouse_fd, 0, SEEK_SET);
    int localMouseX = context->mouseStatus.pos.x - context->viewport->loc.x;
    int localMouseY = context->mouseStatus.pos.y - context->viewport->loc.y - 10;
    if(context == NULL) return;
    //printf("Handle Context\n");
    if(
        !( 
            localMouseX < 0 || localMouseX >= context->viewport->loc.w ||
            localMouseY < 0 || localMouseY >= context->viewport->loc.h
        )
    ){
        gui_processInteraction(localMouseX, localMouseY, context, (struct GUIElement *) context, 0, 0);
    }
    
    gui_drawElement(context, (struct GUIElement *) context, 0, 0);
    //printf("Handle Context Finish\n");

    vp_copy(context->viewport);
}

extern int running;

void gui_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
	if(event == VP_MAXIMIZE/* || event == VP_FOCUSED */){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE/* || event == VP_UNFOCUSED */){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
        set_schedule(ALWAYS);
    }
    else if(event == VP_FOCUSED){
        set_schedule(ALWAYS);
    }
    else if(event == VP_UNFOCUSED){
        set_schedule(NEVER);
    }
}

void drawRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for(int x = x1; x < x2; x++){
        for(int y = y1; y < y2; y++){
            buf[y*buf_width + x] = buf[y*buf_width + x] | innerColor;
            if(x == x1 || y == y1 || x == x2 || y == y2){
                buf[y*buf_width + x] = outerColor;
            }
        }
    }
}

void fillRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for(int x = x1; x <= x2; x++){
        for(int y = y1; y <= y2; y++){
            buf[y*buf_width + x] = innerColor;
            if(x == x1 || y == y1 || x == x2 || y == y2){
                buf[y*buf_width + x] = outerColor;
            }
        }
    }
}
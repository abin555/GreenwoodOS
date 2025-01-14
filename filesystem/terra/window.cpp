#include "application.hpp"
#include "utilities.hpp"

#define Width 300
#define Height 300

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_EXIT){
        sys.running = 0;
        set_schedule(ALWAYS);
    }
	else if(event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
	}
	else if(event == VP_MINIMIZE){
		set_schedule(NEVER);
	}
}

Window::Window(char *name){
    //this->window = window_open(name, 1);
    this->vp = sys.vp_funcs->open(Width, Height, "Terra");
    uint32_t *buf = (uint32_t *) sys.malloc(Width*Height*sizeof(uint32_t));
    sys.vp_funcs->add_event_handler(this->vp, event_handler);
    sys.vp_funcs->set_buffer(this->vp, buf, Width*Height*sizeof(uint32_t));
}
Window::Window(){
    this->window = NULL;
}

void Window::update(){
    //window_update();
    sys.vp_funcs->copy(this->vp);
    //print("Window Update?\n");
}

void Window::close(){
    //window_close(this->window);
    sys.vp_funcs->close(this->vp);
}

int Window::getWidth(){
    //return this->window->width;
    return this->vp->loc.w;
}

int Window::getHeight(){
    //return this->window->height;
    return this->vp->loc.h;
}

uint32_t *Window::getBuffer(){
    //return this->window->backbuffer;
    return this->vp->backbuf;
}

void Window::clear(uint32_t color){
    //memset(this->window->backbuffer, color, this->window->width * this->window->height * sizeof(uint32_t));
    uint32_t *buf = (uint32_t *) this->vp->backbuf;
    int w, h;
    w = getWidth();
    h = getHeight();
    int n = w * h;
    for(int i = 0; i < n; i++){
        buf[i] = color;
    }
}

struct Vec2 Window::getRelativeMouse(){
    struct Vec2 pos;
    pos.x = mouse->pos.x - this->vp->loc.x;
    pos.y = mouse->pos.y - this->vp->loc.y - 8;
    return pos;
}
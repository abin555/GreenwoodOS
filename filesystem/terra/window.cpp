#include "application.hpp"
#include "utilities.hpp"

Window::Window(char *name){
    this->window = window_open(name, 1);
}
Window::Window(){
    this->window = NULL;
}

void Window::update(){
    window_update();
    //print("Window Update?\n");
}

void Window::close(){
    window_close(this->window);
}

int Window::getWidth(){
    return this->window->width;
}

int Window::getHeight(){
    return this->window->height;
}

uint32_t *Window::getBuffer(){
    return this->window->backbuffer;
}

void Window::clear(uint32_t color){
    //memset(this->window->backbuffer, color, this->window->width * this->window->height * sizeof(uint32_t));
    uint32_t *buf = (uint32_t *) this->window->backbuffer;
    for(int i = 0; i < this->window->width * this->window->height; i++){
        buf[i] = color;
    }
}
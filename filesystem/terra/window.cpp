#include "application.hpp"
#include "utilities.hpp"

Window::Window(char *name){
    this->window = window_open(name);
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
    memset(this->window->backbuffer, color, this->window->width * this->window->height * sizeof(uint32_t));
}
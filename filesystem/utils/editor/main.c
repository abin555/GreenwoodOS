#include "libc.h"
#include "gui.h"
#include "memory.h"

int running;

int main(int argc, char **argv){
    memsetup(0x400000);
    gui_setup();
    running = 1;

    struct WindowContext *context = gui_makeContext("Editor", 60*8, 60*8, 10, 0x0);
    struct GUIBar *mainBar = gui_makeBar(context->width, 12, 0xc9c9c9, 0xb9b9b9, 4);
    gui_setLocation(mainBar, 0, 0);
    gui_addChild(context, mainBar);
    struct GUIButton *mainButton = gui_makeButton("Test Button", 0x0, 0xb9b9b9);
    struct GUIButton *exitButton = gui_makeButton("Exit", 0x0, 0xb9b9b9);
    gui_setLocation(mainButton, 8, 2);
    gui_setScale(mainButton, 11*8, 8);
    gui_buttonSetHover(mainButton, 0xb9b9b9, 0x0);

    gui_setLocation(exitButton, 13*8, 2);
    gui_setScale(exitButton, 4*8, 8);
    gui_buttonSetHover(exitButton, 0xb9b9b9, 0x0);

    gui_addChild(mainBar, mainButton);
    gui_addChild(mainBar, exitButton);

    struct GUIScroll *mainScroll = gui_makeScroll(12, 59*8-mainBar->location.h, 30, 0xc9c9c9, 0xb9b9b9, 0x0F0F0F);
    gui_setLocation(mainScroll, 0, 12);
    //dprint("Set Location\n");
    gui_addChild(context, mainScroll);

    while(running){
        gui_handleContext(context);
        if(mainButton->isClicked){
            dprint("Button Click\n");
            mainButton->isClicked = 0;
        }
        if(exitButton->isClicked){
            running = 0;
            exitButton->isClicked = 0;
        }
    }
    gui_closeContext(context);

    memfinish();
}
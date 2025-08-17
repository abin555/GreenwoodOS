#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/task.h>
#include "gui.h"
#include "editor.h"

int running;

int main(int argc, char **argv){
    if(argc != 2){
        printf("Missing file!\n");
        return 1;
    }
    gui_setup();
    running = 1;

    struct EditorFile file = editor_open(argv[1]);
    if(file.file == NULL){
        printf("Unable to open file %s\n", argv[1]);
        return 1;
    }

    struct WindowContext *context = gui_makeContext("Editor", 12+60*8, 60*8, 10, 0x0);
    struct GUIBar *mainBar = gui_makeBar(context->width, 12, 0xc9c9c9, 0xb9b9b9, 4);
    gui_setLocation(mainBar, 0, 0);
    gui_addChild(context, mainBar);
    struct GUIButton *mainButton = gui_makeButton("Test Button", 0x0, 0xb9b9b9);
    struct GUIButton *saveButton = gui_makeButton("Save", 0x0, 0xb9b9b9);
    struct GUIButton *exitButton = gui_makeButton("Exit", 0x0, 0xb9b9b9);
    gui_setLocation(mainButton, 8, 2);
    gui_setScale(mainButton, 11*8, 8);
    gui_buttonSetHover(mainButton, 0xb9b9b9, 0x0);

    gui_setLocation(saveButton, 13*8, 2);
    gui_setScale(saveButton, 6*8, 8);
    gui_buttonSetHover(saveButton, 0xb9b9b9, 0x0);

    gui_setLocation(exitButton, 21*8, 2);
    gui_setScale(exitButton, 4*8, 8);
    gui_buttonSetHover(exitButton, 0xb9b9b9, 0x0);

    gui_addChild(mainBar, mainButton);
    gui_addChild(mainBar, saveButton);
    gui_addChild(mainBar, exitButton);

    struct GUIScroll *mainScroll = gui_makeVScroll(12, 59*8-mainBar->location.h+7, 30, 0xc9c9c9, 0xb9b9b9, 0x0F0F0F);
    gui_setLocation(mainScroll, 0, mainBar->location.h);
    gui_addChild(context, mainScroll);

    struct Location textBox = {13, 12, context->viewport->loc.w - 24, context->viewport->loc.h - 24};

    while(running){
        gui_handleContext(context);
        if(mainButton->isClicked){
            printf("Button Click\n");
            mainButton->isClicked = 0;
        }
        if(saveButton->isClicked){
            editor_save(&file);
            saveButton->isClicked = 0;
        }
        if(exitButton->isClicked){
            running = 0;
            exitButton->isClicked = 0;
        }
        int line = editor_getLineFromFilePct(
            &file,
            mainScroll->scroll
        );
        editor_update(context, &file);
        editor_draw(
            context,
            textBox,
            &file,
            line,
            line + (textBox.h / 8)
        );

        vp_copy(context->viewport);

        yield();
    }
    gui_closeContext(context);
}
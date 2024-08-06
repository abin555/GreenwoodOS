#include "desktop.h"

#define BACKGROUND_FILE "A/image/bliss.tga\0"

#define numIcons 10

int desktop_viewer(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    struct WINDOW *window = window_open("DESKTOP", true);
    struct task_state *window_task = &tasks[task_running_idx];
    window_task->window = window;
    window_task->console = kernel_console;
    set_schedule(ONFOCUS);

    struct Bitmap background = loadBitmap(BACKGROUND_FILE);
    struct Bitmap folder = loadBitmap("A/OS/icons/folder.tga\0");
    struct Bitmap blockDevice = loadBitmap("A/OS/icons/block-device.tga\0");

    struct Icon icons[numIcons];

    for(int i = 0; i < numIcons; i++){
        char folderLabel[5] = {'F','O','L','D',0};
        icons[i] = generateIcon(folder, 25 + i * 35, 25, folder.width, folder.height, folderLabel);
    }
    for(int i = 0; i < drive_count && i < numIcons; i++){
        char driveLabel[2] = {drives[i]->identity, 0};
        icons[i] = generateIcon(blockDevice, 25 + i * 35, 25, blockDevice.width, blockDevice.height, driveLabel);
    }

    struct Viewport testViewport = make_viewport(400,400, "Test VP");

    //exec("/A/tune/tune.exe", 0, NULL);

    struct {
        int startX;
        int startY;
        char dragging;
        int dragType;//0 = icon, 1 = viewport
    } ClickDrag;

    while(1){
        drawBitmap(0, 0, background, window); 
        for(int i = 0; i < numIcons; i++)  
            drawIcon(&icons[i], window);

        draw_viewport(&testViewport, window);

        if(mouseStatus.buttons.left && !ClickDrag.dragging){
            if(viewport_handle_click_event(&testViewport, mouseStatus.pos.x, mouseStatus.pos.y)){
                testViewport.selected = 1;
                testViewport.oldLoc = testViewport.loc;
                ClickDrag.dragType = 1;
            }
            if(ClickDrag.dragType != 1){
                for(int i = 0; i < numIcons; i++){
                    if(getIconHover(&icons[i], mouseStatus.pos.x, mouseStatus.pos.y)){
                        icons[i].selected = 1;
                        icons[i].oldLoc = icons[i].loc;
                        ClickDrag.dragType = 0;
                    }
                }
            }
            
            ClickDrag.dragging = 1;
            ClickDrag.startX = mouseStatus.pos.x;
            ClickDrag.startY = mouseStatus.pos.y;
        }
        if(!mouseStatus.buttons.left && ClickDrag.dragging) {
            ClickDrag.dragging = 0;
            ClickDrag.dragType = -1;
            for(int i = 0; i < numIcons; i++){
                icons[i].selected = 0;
            }
            testViewport.selected = 0;
        }

        if(ClickDrag.dragging){
            //window->backbuffer[(ClickDrag.startY)*window->width + (ClickDrag.startX)] = 0xFF0000;
            //window->backbuffer[(mouseStatus.pos.y)*window->width + (mouseStatus.pos.x)] = 0xFF0000;

            for(int i = 0; i < numIcons; i++){
                if(icons[i].selected){
                    icons[i].loc.x = icons[i].oldLoc.x - (ClickDrag.startX - mouseStatus.pos.x);
                    icons[i].loc.y = icons[i].oldLoc.y - (ClickDrag.startY - mouseStatus.pos.y);
                }
            }
            if(testViewport.selected){
                testViewport.loc.x = testViewport.oldLoc.x - (ClickDrag.startX - mouseStatus.pos.x);
                testViewport.loc.y = testViewport.oldLoc.y - (ClickDrag.startY - mouseStatus.pos.y);
                if(testViewport.loc.x < 0) testViewport.loc.x = 0;
                if(testViewport.loc.y < 0) testViewport.loc.y = 0;
                if(testViewport.loc.x + testViewport.loc.w >= (int) window->width) testViewport.loc.x = window->width - testViewport.loc.w;
                if(testViewport.loc.y + testViewport.loc.h >= (int) window->height) testViewport.loc.y = window->height - testViewport.loc.h;
            }
            /*
            drawRect(
                0x0000FF,
                0x0000DD,
                ClickDrag.startX,
                ClickDrag.startY,
                mouseStatus.pos.x,
                mouseStatus.pos.y,
                window->backbuffer,
                window->width
            );
            */
        }

        window_copy_buffer(window);
    }
}
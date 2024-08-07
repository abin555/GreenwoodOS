#include "desktop.h"

//#define BACKGROUND_FILE "A/image/bliss.tga\0"
#define BACKGROUND_FILE "A/image/Eibsee.tga\0"

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

    struct Viewport viewports[2];
    viewports[0] = make_viewport(400, 400, "VP 1");
    viewports[1] = make_viewport(200, 100, "VP 2");


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

        for(int i = 0; i < 2; i++)
            draw_viewport(&viewports[i], window);

        if(mouseStatus.buttons.left && !ClickDrag.dragging){
            for(int i = 0; i < 2; i++){
                if(viewport_handle_title_click_event(&viewports[i], mouseStatus.pos.x, mouseStatus.pos.y)){
                    viewports[i].selected = 1;
                    viewports[i].oldLoc = viewports[i].loc;
                    ClickDrag.dragType = 1;
                    break;
                }
            }
            if(ClickDrag.dragType != 1){
                for(int i = 0; i < numIcons; i++){
                    if(getIconHover(&icons[i], mouseStatus.pos.x, mouseStatus.pos.y)){
                        icons[i].selected = 1;
                        icons[i].oldLoc = icons[i].loc;
                        ClickDrag.dragType = 0;
                        break;
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
            for(int i = 0; i < 2; i++)
                viewports[i].selected = 0;
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
            for(int i = 0; i < 2; i++){
                if(viewports[i].selected){
                    viewports[i].loc.x = viewports[i].oldLoc.x - (ClickDrag.startX - mouseStatus.pos.x);
                    viewports[i].loc.y = viewports[i].oldLoc.y - (ClickDrag.startY - mouseStatus.pos.y);
                    if(viewports[i].loc.x < 0) viewports[i].loc.x = 0;
                    if(viewports[i].loc.y < 0) viewports[i].loc.y = 0;
                    if(viewports[i].loc.x + viewports[i].loc.w >= (int) window->width) viewports[i].loc.x = window->width - viewports[i].loc.w;
                    if(viewports[i].loc.y + viewports[i].loc.h >= (int) window->height) viewports[i].loc.y = window->height - viewports[i].loc.h;
                }
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
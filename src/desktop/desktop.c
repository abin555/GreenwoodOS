#include "desktop.h"

#define BACKGROUND_FILE "A/image/norway.tga\0"
//#define BACKGROUND_FILE "A/image/bliss.tga\0"
//#define BACKGROUND_FILE "A/image/Eibsee.tga\0"

#define numIcons 10

int desktop_viewer(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    struct WINDOW *window = window_open("DESKTOP", true);
    struct task_state *window_task = &tasks[task_running_idx];
    window_task->keyboard_event_handler = desktop_kbd_event;
    window_task->window = window;
    window_task->console = kernel_console;
    set_schedule(ALWAYS);

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

    print_serial("[DESKTOP] Global Viewport is at %x\n", &global_viewport_list);
    print_serial("[DESKTOP] Global Viewport points to %x\n", global_viewport_list);
    void *vp_list_ptr = malloc(sizeof(struct ViewportList));
    global_viewport_list = (struct ViewportList*) vp_list_ptr;
    print_serial("[DESKTOP] VP LIST pointer is %x\n", vp_list_ptr);
    print_serial("[DESKTOP] Global Viewport points to %x\n", global_viewport_list);

    viewport_init_sys(global_viewport_list);

    

    //exec("/A/tune/tune.exe", 0, NULL);
    exec("/A/OS/termvp/term.exe", 0, NULL);

    struct {
        int startX;
        int startY;
        char dragging;
        int dragType;//0 = icon, 1 = viewport
        struct Viewport *selected_vp;
    } ClickDrag;

    set_schedule(ONFOCUS);

    while(1){
        drawBitmap(0, 0, background, window); 
        for(int i = 0; i < numIcons; i++)  
            drawIcon(&icons[i], window);
        viewport_draw_all(global_viewport_list, window);

        if(mouseStatus.buttons.left && !ClickDrag.dragging){
            struct Viewport_Interaction vp_interaction = viewport_process_click(global_viewport_list, mouseStatus.pos.x, mouseStatus.pos.y);
            if(vp_interaction.clickType == VP_Close && vp_interaction.vp != NULL){
                viewport_close(global_viewport_list, vp_interaction.vp);
            }
            else if(vp_interaction.clickType == VP_Header && vp_interaction.vp != NULL){
                ClickDrag.selected_vp = vp_interaction.vp;
                ClickDrag.selected_vp->oldLoc = ClickDrag.selected_vp->loc;
            }
            else if(vp_interaction.clickType == VP_None){
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
            ClickDrag.selected_vp = NULL;
        }

        if(ClickDrag.dragging){
            for(int i = 0; i < numIcons; i++){
                if(icons[i].selected){
                    icons[i].loc.x = icons[i].oldLoc.x - (ClickDrag.startX - mouseStatus.pos.x);
                    icons[i].loc.y = icons[i].oldLoc.y - (ClickDrag.startY - mouseStatus.pos.y);
                }
            }
            if(ClickDrag.selected_vp != NULL){
                viewport_set_position(ClickDrag.selected_vp, window,
                    ClickDrag.selected_vp->oldLoc.x - (ClickDrag.startX - mouseStatus.pos.x),
                    ClickDrag.selected_vp->oldLoc.y - (ClickDrag.startY - mouseStatus.pos.y)
                );
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

void desktop_kbd_event(char ascii){
    //print_serial("[DESKTOP] Kbd callback - %c\n", (char) ascii);
    if(KBD_flags.ctrl && ascii == 'T'){
        exec("/A/OS/termvp/term.exe", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'M'){
        for(int i = 0; i < global_viewport_list->count; i++){
            if(global_viewport_list->elements[i].inUse){
                if(!global_viewport_list->elements[i].vp->minimized)
                    viewport_toggle_size(global_viewport_list->elements[i].vp);
            }
        }
    }
    else if(global_viewport_list->elements[0].inUse){
        if(global_viewport_list->elements[0].vp == NULL) return;
        if(!global_viewport_list->elements[0].vp->minimized){
            global_viewport_list->elements[0].vp->ascii = (char) ascii;
        }
    }
}
#include "desktop.h"
#include "vfs.h"
#include "sysfs.h"

#define BACKGROUND_FILE "A/Pictures/norway.tga\0"
//#define BACKGROUND_FILE "A/image/bliss.tga\0"
//#define BACKGROUND_FILE "A/image/Eibsee.tga\0"

#define numIcons 0

void __attribute__ ((optimize("-O3"))) drawBackground(struct Bitmap bitmap, struct WINDOW *window){
    if(bitmap.bitmap == NULL || window == NULL) return;
    if(bitmap.width == window->width){
        memfcpy(window->backbuffer, bitmap.bitmap, bitmap.width * bitmap.height * sizeof(uint32_t));
    }
    else{
        for(uint32_t i = 0; i < bitmap.height; i++){
            memfcpy(
                window->backbuffer + i*window->width, 
                bitmap.bitmap + i*bitmap.width,
                bitmap.width * sizeof(uint32_t)
            );
        }
    }
}

struct Bitmap background;

void desktopbg_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *bg = cdev;
    print_serial("[DESKTOP] Write to background of size %d at offset %d @ 0x%x\n", nbytes, offset, bg->buf);
    *head = 0;
}

int __attribute__ ((optimize("-O3"))) desktop_viewer(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    struct WINDOW *window = window_open("DESKTOP", true);
    struct task_state *window_task = &tasks[task_running_idx];
    window_task->keyboard_event_handler = desktop_kbd_event;
    window_task->window = window;
    window_task->console = kernel_console;
    set_schedule(ALWAYS);
    print_console(kernel_console, "Starting Desktop Environment!\n");

    background = loadBitmap(BACKGROUND_FILE);
    struct Bitmap blockDevice = loadBitmap("A/OS/icons/block-device.tga\0");
    
    struct VFS_Inode *vfs_sysroot = vfs_findRoot('-');
    if(vfs_sysroot->type == VFS_SYS){
        struct SysFS_Inode *sysfs = vfs_sysroot->fs.sysfs;
        struct SysFS_Inode *desktopCDEV = sysfs_mkcdev(
            "desktopBG",
            sysfs_createCharDevice(
                (char *) &background,
                sizeof(background),
                CDEV_READ | CDEV_WRITE
            )
        );
        sysfs_setCallbacks(desktopCDEV->data.chardev, (void (*)(void *, int offset, int nbytes, int *head)) desktopbg_write_callback, NULL, NULL, NULL);
        sysfs_addChild(sysfs_find(sysfs, "sys\0"), desktopCDEV);
        sysfs_debugTree(sysfs, 0);        
    }

    struct Icon icons[numIcons];
    memset(icons, 0, sizeof(struct Icon) * numIcons);
    
    /*
    for(int i = 0; i < numIcons; i++){
        char folderLabel[5] = {'F','O','L','D',0};
        icons[i] = generateIcon(folder, 25 + i * 35, 25, folder.width, folder.height, folderLabel);
    }
    */
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

    /*
    struct Viewport *kernel_console_vp = viewport_indirect_open(20*8, 40*8, "Kernel Console");
    viewport_set_buffer(kernel_console_vp, malloc(20*8*40*8*sizeof(uint32_t)), 20*8*40*8);
    console_close(kernel_console);
    kernel_console = console_open_vp(kernel_console_vp);
    print_console(kernel_console, "TEST CONSOLE!\n");
    */

    exec("/A/utils/explorer/explorer.elf", 0, NULL);
    //exec("/A/utils/clock/clock.exe", 0, NULL);

    //exec("/A/clock/clock.exe", 0, NULL);

    struct {
        int startX;
        int startY;
        char dragging;
        int dragType;//0 = icon, 1 = viewport
        struct Viewport *selected_vp;
    } ClickDrag = {
        0,
        0,
        0,
        -1,
        NULL
    };

    set_schedule(ONFOCUS);

    while(1){
        task_lock = 1;
        drawBackground(background, window);
        
        for(int i = 0; i < numIcons; i++)  
            drawIcon(&icons[i], window);
        
        viewport_draw_all(global_viewport_list, window);
        task_lock = 0;

        if(mouseStatus.buttons.left && !ClickDrag.dragging){
            struct Viewport_Interaction vp_interaction = viewport_process_click(global_viewport_list, mouseStatus.pos.x, mouseStatus.pos.y);
            if(vp_interaction.clickType == VP_Close && vp_interaction.vp != NULL){
                viewport_close(global_viewport_list, vp_interaction.vp);
            }
            else if(vp_interaction.clickType == VP_Header && vp_interaction.vp != NULL){
                ClickDrag.selected_vp = vp_interaction.vp;
                ClickDrag.selected_vp->oldLoc = ClickDrag.selected_vp->loc;
            }
            ClickDrag.dragging = 1;
            ClickDrag.startX = mouseStatus.pos.x;
            ClickDrag.startY = mouseStatus.pos.y;
        }
        if(!mouseStatus.buttons.left && ClickDrag.dragging) {
            ClickDrag.dragging = 0;
            ClickDrag.dragType = -1;
            ClickDrag.selected_vp = NULL;
        }

        if(ClickDrag.dragging){
            if(ClickDrag.selected_vp != NULL){
                viewport_set_position(ClickDrag.selected_vp, window,
                    ClickDrag.selected_vp->oldLoc.x - (ClickDrag.startX - mouseStatus.pos.x),
                    ClickDrag.selected_vp->oldLoc.y - (ClickDrag.startY - mouseStatus.pos.y)
                );
            }
            else{
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
            
            
        }

        window_copy_buffer(window);
    }
}

void __attribute__ ((optimize("-O3"))) desktop_kbd_event(char ascii){
    //print_serial("[DESKTOP] Kbd callback - %c\n", (char) ascii);
    if(KBD_flags.ctrl && ascii == 'T'){
        exec("/A/OS/termvp/term.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'E'){
        exec("/A/utils/explorer/explorer.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'C'){
        exec("/A/utils/clock/clock.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'L'){
        exec("/A/lisp/LISP.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'M'){
        for(int i = 0; i < global_viewport_list->count; i++){
            if(global_viewport_list->elements[i].inUse){
                if(!global_viewport_list->elements[i].vp->minimized)
                    viewport_toggle_size(global_viewport_list->elements[i].vp);
            }
        }
    }
    else if(KBD_flags.ctrl && ascii == 'V'){
        exec("/A/utils/vaporwave/vaporwave.elf", 0, NULL);
    }
    else if(global_viewport_list->elements[0].inUse){
        if(global_viewport_list->elements[0].vp == NULL) return;
        if(!global_viewport_list->elements[0].vp->minimized){
            global_viewport_list->elements[0].vp->ascii = (char) ascii;
        }
    }
}
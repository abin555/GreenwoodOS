#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/window.h>
#include <sys/sysfs.h>

#include "kbd.h"
#include "viewport.h"
#include "icon.h"
#include "gfx.h"
#include "bitmap.h"
#include "hooks.h"

#define BACKGROUND_FILE "/A/Pictures/norway.tga\0"

void addKbdEventHandler(void (*handler)(void *)){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) handler;
	eax = 0x21;
	asm("int $0x80");
}

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

void desktop_kbd_event(struct KBD_flags *flags);

struct WINDOW *desktop_win;

struct Bitmap background;
struct DesktopConfig desktopConfig;

struct MouseButtons{
    uint8_t right : 1;
    uint8_t left : 1;
    uint8_t middle : 1;
};

struct IVec2{
    int x;
    int y;
};

struct MouseStatus{
    struct IVec2 pos;
    struct MouseButtons buttons;
    struct IVec2 lastDelta;
};

void desktopbg_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *bg = cdev;
    print_serial("[DESKTOP] Write to background of size %d at offset %d @ 0x%x\n", nbytes, offset, bg->buf);
    *head = 0;
}

void cursorbmap_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *cursor = cdev;
    print_serial("[DESKTOP] [CURSOR] Write to cursor of size %d at offset %d @ 0x%x\n", nbytes, offset, cursor->buf);
    *head = 0;
}

void screen_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *screen = cdev;
    print_serial("[DESKTOP] [SCREEN] Write to screen of size %d at offset %d @ 0x%x\nROOT X: %d ROOT Y: %d\n", nbytes, offset, screen->buf, desktopConfig.vp_root_x, desktopConfig.vp_root_y);
    *head = 0;
}

int main(int argc, char **argv){
    desktop_win = window_open("DESKTOP", true);
    addKbdEventHandler((void (*)(void *)) desktop_kbd_event);

    hooks_attach();

    set_schedule(ALWAYS);
    printf("Starting Desktop Environment\n");

    background = loadBitmap(BACKGROUND_FILE);
    struct Bitmap blockDevice = loadBitmap("A/OS/icons/block-device.tga\0");

    desktopConfig = (struct DesktopConfig) {
        10, 10, //VP origins are at (10,10)
        //Screen dimensions
        desktop_win->width,
        desktop_win->height
    };

    void *vp_list_ptr = malloc(sizeof(struct ViewportList));
    global_viewport_list = (struct ViewportList*) vp_list_ptr;

    viewport_init_sys(global_viewport_list);

    struct MouseStatus mouseStatus;

    int mouse_fd = open("/-/sys/mouse", 1);
    if(mouse_fd == -1){
        printf("Cannot open mouse file\n");
        return 1;
    }

    FILE *metaFile = fopen("/-/fsCTRL", "r");
    if(metaFile == NULL){
        printf("Cannot open sysfs metafile\n");
        return 1;
    }
    struct SysFS_Meta meta;
    fread(&meta, sizeof(meta), 1, metaFile);
    fclose(metaFile);

    struct SysFS_Inode *sysfs = meta.root;

    struct SysFS_Inode *desktopCDEV = meta.mkcdev(
        "desktopBG",
        meta.createCdev(
            (char *) &background,
            sizeof(background),
            CDEV_READ | CDEV_WRITE
        )
    );
    meta.setCallbacks(desktopCDEV->data.chardev, (void (*)(void *, int offset, int nbytes, int *head)) desktopbg_write_callback, NULL, NULL, NULL);
    meta.addChild(meta.find(sysfs, "sys\0", NULL), desktopCDEV);
    /*
    struct SysFS_Inode *cursorCDEV = sysfs_mkcdev(
        "cursorBM",
        sysfs_createCharDevice(
            (char *) &cursor_bitmap_s,
            sizeof(cursor_bitmap_s),
            CDEV_READ | CDEV_WRITE
        )
    );
    sysfs_setCallbacks(cursorCDEV->data.chardev, (void (*)(void *, int offset, int nbytes, int *head)) cursorbmap_write_callback, NULL, NULL, NULL);
    sysfs_addChild(meta.find(sysfs, "sys\0", NULL), cursorCDEV);
    */
    struct SysFS_Inode *screenCDEV = meta.mkcdev(
        "screen",
        meta.createCdev(
            (char *) &desktopConfig,
            sizeof(desktopConfig),
            CDEV_READ | CDEV_WRITE
        )
    );
    meta.setCallbacks(screenCDEV->data.chardev, (void (*)(void *, int offset, int nbytes, int *head)) screen_write_callback, NULL, NULL, NULL);
    meta.addChild(meta.find(sysfs, "sys\0", NULL), screenCDEV);
    struct SysFS_Inode *viewport = meta.mkcdev("viewport", 
        meta.createCdev(
            (char *) &global_viewport_functions,
            sizeof(global_viewport_functions),
            CDEV_READ
        )
    );
    meta.addChild(meta.find(sysfs, "sys\0", NULL), viewport);
    //sysfs_debugTree(sysfs, 0);   

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
        task_lock(1);
        lseek(mouse_fd, 0, SEEK_SET);
        read(mouse_fd, &mouseStatus, sizeof(mouseStatus));
        drawBackground(background, desktop_win);
        
        viewport_draw_all(global_viewport_list, desktop_win);
        task_lock(0);

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
                viewport_set_position(ClickDrag.selected_vp, desktop_win,
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

        window_update();

        yield();
    }
}

void __attribute__ ((optimize("-O3"))) desktop_kbd_event(struct KBD_flags *flags){
    char ascii = flags->key;
    //print_serial("[DESKTOP] Kbd callback - %c\n", (char) ascii);
    if(flags->ctrl && ascii == 'T'){
        exec("/A/OS/termvp/term.elf", 0, NULL);
    }
    else if(flags->ctrl && ascii == 'E'){
        exec("/A/utils/explorer/explorer.elf", 0, NULL);
    }
    else if(flags->ctrl && ascii == 'Q'){
        exec("/A/utils/task/taskmgr.elf", 0, NULL);
    }
    else if(flags->ctrl && ascii == 'C'){
        exec("/A/utils/clock/clock.elf", 0, NULL);
    }
    else if(flags->ctrl && ascii == 'L'){
        exec("/A/lisp/lisp.elf", 0, NULL);
    }
    else if(flags->ctrl && ascii == 'M'){
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
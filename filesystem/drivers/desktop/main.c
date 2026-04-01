#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/window.h>
#include <sys/task.h>
#include <sys/mouse.h>
#include <string.h>
#include <sys/sysfs.h>
#include <sys/keyboard.h>
#include <sys/task.h>
#include <sys/io.h>
#include <sys/mouse.h>
#include <sys/sysfs.h>

#include "bitmap.h"
#include "desktop_shared.h"
#include "gfx.h"
#include "viewport.h"
#include "hooks.h"

#define BACKGROUND_FILE "/A/Pictures/skyline.tga\0"

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
struct DesktopConfig desktopConfig;

void desktopbg_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *bg = cdev;
    printf("[DESKTOP] Write to background of size %d at offset %d @ 0x%x\n", nbytes, offset, bg->buf);
    *head = 0;
}

void cursorbmap_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *cursor = cdev;
    printf("[DESKTOP] [CURSOR] Write to cursor of size %d at offset %d @ 0x%x\n", nbytes, offset, cursor->buf);
    *head = 0;
}

void screen_write_callback(void *cdev, int offset, int nbytes, int *head){
    struct SysFS_Chardev *screen = cdev;
    printf("[DESKTOP] [SCREEN] Write to screen of size %d at offset %d @ 0x%x\nROOT X: %d ROOT Y: %d\n", nbytes, offset, screen->buf, desktopConfig.vp_root_x, desktopConfig.vp_root_y);
    *head = 0;
}

void __attribute__ ((optimize("-O3"))) desktop_kbd_event(struct KBD_flags *flags, char ascii){
    //print_serial("[DESKTOP] Kbd callback - %c\n", (char) ascii);
    if(flags->ctrl && ascii == 'T'){
        exec("/A/utils/term2/term2.elf", 0, NULL);
    }
    else if(flags->ctrl && ascii == 'E'){
        exec("/A/utils/explorer/explorer.elf", 0, NULL);
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

int main(int argc, char **argv){
    hooks_load();
    struct WINDOW *win = window_open("Desktop", 0);
    set_schedule(ONFOCUS);

    FILE *mouseFile = fopen("/-/sys/mouse", "r");
    struct MouseStatus mouse;

    uint32_t bufsize = win->width * win->height * sizeof(uint32_t);
    background = loadBigBitmap(BACKGROUND_FILE, win->width, win->height);

    desktopConfig = (struct DesktopConfig) {
        10, 10, //VP origins are at (10,10)
        //Screen dimensions
        win->width,
        win->height
    };

    printf("[DESKTOP] Global Viewport is at %x\n", &global_viewport_list);
    printf("[DESKTOP] Global Viewport points to %x\n", global_viewport_list);
    void *vp_list_ptr = malloc(sizeof(struct ViewportList));
    global_viewport_list = (struct ViewportList*) vp_list_ptr;
    printf("[DESKTOP] VP LIST pointer is %x\n", vp_list_ptr);
    printf("[DESKTOP] Global Viewport points to %x\n", global_viewport_list);

    viewport_init_sys(global_viewport_list, win);

    FILE *metaFile = fopen("/-/fsCTRL", "r");
    if(metaFile == NULL){
        printf("Cannot open file!\n");
        return 1;
    }
    struct SysFS_Meta meta;
    fread(&meta, sizeof(meta), 1, metaFile);
    fclose(metaFile);

    struct SysFS_Inode *sys = meta.find(meta.root, "sys\0", NULL);
    if(sys != NULL){
        struct SysFS_Inode *desktopCDEV = meta.mkcdev(
            "desktopBG",
            meta.createCdev(
                (char *) &background,
                sizeof(background),
                CDEV_READ | CDEV_WRITE
            )
        );
        meta.setCallbacks(desktopCDEV->data.chardev, (void (*)(void *, int offset, int nbytes, int *head)) desktopbg_write_callback, NULL, NULL, NULL);
        meta.addChild(sys, desktopCDEV);
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
        sysfs_addChild(sys, cursorCDEV);
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
        meta.addChild(sys, screenCDEV);

        struct SysFS_Inode *viewport = meta.mkcdev("viewport", 
            meta.createCdev(
                (char *) &global_viewport_functions,
                sizeof(global_viewport_functions),
                CDEV_READ
            )
        );
        meta.addChild(sys, viewport);
    }

    struct {
        int startX;
        int startY;
        char dragging;
        int dragType;//0 = icon, 1 = viewport, 2 = resize
        struct Viewport *selected_vp;
    } ClickDrag = {
        0,
        0,
        0,
        -1,
        NULL
    };

    set_schedule(ONFOCUS);

    struct KBD_flags KBD_bak;
    struct KBD_flags KBD;

    int kbd_flags_fd = open("/-/sys/kbdFlags", O_READ);
    lseek(kbd_flags_fd, 0, 0);
    read(kbd_flags_fd, (void *) &KBD_bak, sizeof(KBD_bak));

    while(1){
        task_lock(1);
        lseek(kbd_flags_fd, 0, 0);
        read(kbd_flags_fd, (void *) &KBD, sizeof(KBD));
        if(KBD.tick != KBD_bak.tick){
            desktop_kbd_event(&KBD, KBD.key);
            KBD_bak = KBD;
        }

        drawBackground(background, win);
        viewport_draw_all(global_viewport_list, win);

        if(mouse.buttons.left && !ClickDrag.dragging){
            struct Viewport_Interaction vp_interaction = viewport_process_click(global_viewport_list, mouse.pos.x, mouse.pos.y);
            if(vp_interaction.clickType == VP_Close && vp_interaction.vp != NULL){
                viewport_close(global_viewport_list, vp_interaction.vp);
            }
            else if(vp_interaction.clickType == VP_Header && vp_interaction.vp != NULL){
                ClickDrag.selected_vp = vp_interaction.vp;
                ClickDrag.selected_vp->oldLoc = ClickDrag.selected_vp->loc;
                ClickDrag.dragType = 1;
            }
            else if(vp_interaction.clickType == VP_Scale && vp_interaction.vp != NULL){
                ClickDrag.selected_vp = vp_interaction.vp;
                ClickDrag.dragType = 2;
            }
            ClickDrag.dragging = 1;
            ClickDrag.startX = mouse.pos.x;
            ClickDrag.startY = mouse.pos.y;
        }
        if(!mouse.buttons.left && ClickDrag.dragging) {
            if(ClickDrag.selected_vp != NULL && ClickDrag.dragType == 2){
                ClickDrag.selected_vp->resizeLoc.w = mouse.pos.x - ClickDrag.selected_vp->loc.x;
                ClickDrag.selected_vp->resizeLoc.h = mouse.pos.y - ClickDrag.selected_vp->loc.y;
                viewport_send_event(ClickDrag.selected_vp, VP_RESIZE);
            }
            ClickDrag.dragging = 0;
            ClickDrag.dragType = -1;
            ClickDrag.selected_vp = NULL;
        }

        if(ClickDrag.dragging){
            if(ClickDrag.selected_vp != NULL && ClickDrag.dragType == 1){
                viewport_set_position(ClickDrag.selected_vp, win,
                    ClickDrag.selected_vp->oldLoc.x - (ClickDrag.startX - mouse.pos.x),
                    ClickDrag.selected_vp->oldLoc.y - (ClickDrag.startY - mouse.pos.y)
                );
            }
            else if(ClickDrag.dragType == 2){
                drawRect(
                    0xFF0000,
                    0x000000,
                    ClickDrag.selected_vp->loc.x,
                    ClickDrag.selected_vp->loc.y,
                    mouse.pos.x,
                    mouse.pos.y,
                    win->backbuffer,
                    win->width
                );
            }
            else if(ClickDrag.dragType == 1){
                drawRect(
                    0x0000FF,
                    0x0000DD,
                    ClickDrag.startX,
                    ClickDrag.startY,
                    mouse.pos.x,
                    mouse.pos.y,
                    win->backbuffer,
                    win->width
                );
            }
        }

        fseek(mouseFile, 0, SEEK_SET);
        fread(&mouse, sizeof(mouse), 1, mouseFile);

        window_draw_cursor(mouse.pos.x, mouse.pos.y);
        
        window_update();
        task_lock(0);
        yield();
    }

    window_close(win);
}
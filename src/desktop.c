#include "desktop.h"

#define BACKGROUND_FILE "A/image/bliss.tga\0"

typedef struct {
  unsigned char magic1;             // must be zero
  unsigned char colormap;           // must be zero
  unsigned char encoding;           // must be 2
  unsigned short cmaporig, cmaplen; // must be zero
  unsigned char cmapent;            // must be zero
  unsigned short x;                 // must be zero
  unsigned short y;                 // image's height
  unsigned short w;                 // image's height
  unsigned short h;                 // image's width
  unsigned char bpp;                // must be 32
  unsigned char pixeltype;          // must be 40
} __attribute__((packed)) tga_header_t;

struct Bitmap{
    uint8_t *file;
    uint32_t *bitmap;
    uint32_t width;
    uint32_t height;
};

struct Bitmap loadIcon(char *filename){
    struct Bitmap bitmap = {
        NULL,
        NULL,
        0,
        0
    };
    struct FILE *file = fopen(filename);
    if(file == NULL) return bitmap;
    int size = fsize(file);

    bitmap.file = malloc(size+100);
    fcopy(file, (char *) bitmap.file, size);
    fclose(file);
    tga_header_t *header = ((tga_header_t *) bitmap.file);
    bitmap.width = header->w;
    bitmap.height = header->h;
    bitmap.bitmap = (uint32_t *) (bitmap.file + sizeof(tga_header_t) + header->magic1);
    print_serial("[DESKTOP] Loaded Icon %s - W: %d H: %d\n", filename, bitmap.width, bitmap.height);
    
    return bitmap;
}

void drawRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for(int x = x1; x <= x2; x++){
        for(int y = y1; y <= y2; y++){
            buf[y*buf_width + x] = buf[y*buf_width + x] | innerColor;
            if(x == x1 || y == y1 || x == x2 || y == y2){
                buf[y*buf_width + x] = outerColor;
            }
        }
    }
}

void fillRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
){
    if(x1 > x2){
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2){
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for(int x = x1; x <= x2; x++){
        for(int y = y1; y <= y2; y++){
            buf[y*buf_width + x] = innerColor;
            if(x == x1 || y == y1 || x == x2 || y == y2){
                buf[y*buf_width + x] = outerColor;
            }
        }
    }
}

void drawBitmap(int x, int y, struct Bitmap bitmap, struct WINDOW *window){
    if(bitmap.bitmap == NULL || window == NULL) return;
    for(uint32_t ly = 0; ly < bitmap.height; ly++){
        for(uint32_t lx = 0; lx < bitmap.width; lx++){
            uint32_t color = bitmap.bitmap[lx+ly*bitmap.width];
            if(!(color & 0xFF000000)) continue;
            window->backbuffer[(y + ly)*window->width + (x + lx)] = color;
        }
    }
}

struct Location {
    int x;
    int y;
    int w;
    int h;
};

struct Icon {
    struct Bitmap bitmap;
    struct Location loc;
    struct Location oldLoc;
    char label[5];
    bool selected;
};

struct Icon generateIcon(struct Bitmap bitmap, int x, int y, int w, int h, char *label){
    struct Icon new_icon = {
        bitmap,
        {
            x,
            y,
            w,
            h
        },
        {
            x,
            y,
            w,
            h
        },
        "\0\0\0\0\0",
        0
    };

    for(int i = 0; i < 5 && label[i] != '\0'; i++){
        new_icon.label[i] = label[i];
    }

    return new_icon;
}

bool getIconHover(struct Icon *icon, int x, int y){
    if(icon == NULL) return false;
    if(x > icon->loc.x && x < icon->loc.x + icon->loc.w && y > icon->loc.y && y < icon->loc.y + icon->loc.h) return true;
    return false;
}

void drawIcon(struct Icon *icon, struct WINDOW *window){
    if(icon == NULL) return;
    drawBitmap(
        icon->loc.x,
        icon->loc.y,
        icon->bitmap,
        window
    );
    if(icon->selected){
        drawRect(
            0x0000FF,
            0x0000BB,
            icon->loc.x,
            icon->loc.y,
            icon->loc.x + icon->loc.w,
            icon->loc.y + icon->loc.h,
            window->backbuffer, 
            window->width
        );
    }
    for(int i = 0; i < 5 && icon->label[i] != 0; i++){
        buf_putChar(window->backbuffer, icon->loc.x+i*8, icon->loc.y+icon->loc.h, icon->label[i], 0xFFFFFF, 0x0);
    }
}


struct Viewport {
    struct Location loc;
    struct Location oldLoc;
    uint32_t *buf;
    uint32_t buf_size;
    uint8_t resizeable;
    bool selected;
    char *title;
};

bool getViewportHover(struct Viewport *viewport, int x, int y){
    if(viewport == NULL) return false;
    if(x > viewport->loc.x && x < viewport->loc.x + viewport->loc.w && y > viewport->loc.y && y < viewport->loc.y + 10) return true;
    return false;
}

struct Viewport make_viewport(int w, int h, char *title){
    struct Viewport viewport;
    viewport.loc.x = 50;
    viewport.loc.y = 50;
    viewport.loc.w = w;
    viewport.loc.h = h;
    viewport.resizeable = 1;
    viewport.selected = 0;
    viewport.title = title;
    return viewport;
}

void draw_viewport(struct Viewport *viewport, struct WINDOW *window){
    if(viewport == NULL || window == NULL) return;
    fillRect(
        0x0,
        0xBBBBBB,
        viewport->loc.x,
        viewport->loc.y,
        viewport->loc.x + viewport->loc.w,
        viewport->loc.y + 10,
        window->backbuffer,
        window->width
    );
    fillRect(
        0x0,
        0x222222,
        viewport->loc.x,
        viewport->loc.y+10,
        viewport->loc.x + viewport->loc.w,
        viewport->loc.y + viewport->loc.h,
        window->backbuffer,
        window->width
    );
    for(int i = 0; viewport->title[i] != '\0' && viewport->title != NULL; i++){
        buf_putChar(
            window->backbuffer,
            viewport->loc.x+i*8+2,
            viewport->loc.y+2,
            viewport->title[i],
            0x0,
            0xBBBBBB
        );
    } 
}

int desktop_viewer(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    struct WINDOW *window = window_open("DESKTOP", true);
    struct task_state *window_task = &tasks[task_running_idx];
    window_task->window = window;
    window_task->console = kernel_console;
    set_schedule(ONFOCUS);

    struct Bitmap background = loadIcon(BACKGROUND_FILE);
    struct Bitmap folder = loadIcon("A/OS/icons/folder.tga\0");
    struct Bitmap blockDevice = loadIcon("A/OS/icons/block-device.tga\0");

    int numIcons = drive_count + 2;
    struct Icon *icons = malloc(sizeof(struct Icon) * numIcons);

    for(int i = 0; i < numIcons; i++){
        char folderLabel[2] = {0, 0};
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
            if(getViewportHover(&testViewport, mouseStatus.pos.x, mouseStatus.pos.y)){
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
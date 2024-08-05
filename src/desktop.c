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

struct Icon {
    struct Bitmap bitmap;
    struct {
        int x;
        int y;
        int w;
        int h;
    } loc;
};

struct Icon generateIcon(struct Bitmap bitmap, int x, int y, int w, int h){
    struct Icon new_icon = {
        bitmap,
        {
            x,
            y,
            w,
            h
        }
    };
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
}


int desktop_viewer(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    struct WINDOW *window = window_open("DESKTOP", true);
    struct task_state *window_task = &tasks[task_running_idx];
    window_task->window = window;
    window_task->console = kernel_console;
    set_schedule(ONFOCUS);

    struct Bitmap background = loadIcon(BACKGROUND_FILE);
    struct Bitmap folder = loadIcon("A/OS/icons/folder.tga\0");

    struct Icon icons[5];
    for(int i = 0; i < 5; i++){
        icons[i] = generateIcon(folder, 25 + i * 35, 25, folder.width, folder.height);
    }

    exec("/A/tune/tune.exe", 0, NULL);

    struct {
        int startX;
        int startY;
        char dragging;
    } ClickDrag;

    while(1){
        drawBitmap(0, 0, background, window); 
        for(int i = 0; i < 5; i++)  
            drawIcon(&icons[i], window);

        if(mouseStatus.buttons.left && !ClickDrag.dragging){
            ClickDrag.dragging = 1;
            ClickDrag.startX = mouseStatus.pos.x;
            ClickDrag.startY = mouseStatus.pos.y;
        }
        if(!mouseStatus.buttons.left && ClickDrag.dragging) ClickDrag.dragging = 0;

        if(ClickDrag.dragging){
            //window->backbuffer[(ClickDrag.startY)*window->width + (ClickDrag.startX)] = 0xFF0000;
            //window->backbuffer[(mouseStatus.pos.y)*window->width + (mouseStatus.pos.x)] = 0xFF0000;

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
        }

        window_copy_buffer(window);
    }
}
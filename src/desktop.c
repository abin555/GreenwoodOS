#include "desktop.h"

#define BACKGROUND_FILE "A/image/bliss.tga"

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

void drawRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
);

int desktop_viewer(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    struct WINDOW *window = window_open("DESKTOP", true);
    struct task_state *window_task = &tasks[task_running_idx];
    window_task->window = window;
    set_schedule(ONFOCUS);

    uint8_t *background_file = NULL;
    uint32_t *background_bitmap = NULL;
    uint32_t background_width;
    uint32_t background_height;

    if(!fexists(BACKGROUND_FILE)){
        return 1;
    }

    print_serial("[DESKTOP] Loading background image\n");
    struct FILE *background = fopen(BACKGROUND_FILE);
    int size = fsize(background);
    background_file = malloc(size+100);
    fcopy(background, (char *) background_file, size);
    fclose(background);
    tga_header_t *header = ((tga_header_t *) background_file);
    background_width = header->w;
    background_height = header->h;
    background_bitmap = (uint32_t *) (background_file + sizeof(tga_header_t) + header->magic1);
    print_serial("[DESKTOP] BG W: %d H: %d BM: 0x%x\n", background_width, background_height, background_bitmap);

    struct {
        int startX;
        int startY;
        char dragging;
    } ClickDrag;

    while(1){
        for(uint32_t ly = 0; ly < background_height-8; ly++){
            for(uint32_t lx = 0; lx < background_width; lx++){
                uint32_t color = background_bitmap[lx+ly*background_width];
                if(!(color & 0xFF000000)) continue;
                window->backbuffer[(ly)*window->width + (lx)] = color;
            }
        }

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
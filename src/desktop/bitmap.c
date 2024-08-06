#include "bitmap.h"

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


struct Bitmap loadBitmap(char *filename){
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
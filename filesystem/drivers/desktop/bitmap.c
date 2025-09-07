#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/window.h>
#include "bitmap.h"
#include "desktop_shared.h"

#define O_READ 1

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
    int file = open(filename, O_READ);
    if(file == -1) return bitmap;
    int size = lseek(file, 0, 2);
    lseek(file, 0, 0);

    bitmap.file = malloc(size+100);
    read(file, (char *) bitmap.file, size);
    close(file);
    tga_header_t *header = ((tga_header_t *) bitmap.file);
    bitmap.width = header->w;
    bitmap.height = header->h;
    bitmap.bitmap = (uint32_t *) (bitmap.file + sizeof(tga_header_t) + header->magic1);
    printf("[DESKTOP] Loaded Icon %s - W: %d H: %d\n", filename, bitmap.width, bitmap.height);
    
    return bitmap;
}


void __attribute__ ((optimize("-O3"))) drawBitmap(int x, int y, struct Bitmap *bitmap, struct WINDOW *window){
    if(bitmap->bitmap == NULL || window == NULL) return;
    for(uint32_t ly = 0; ly < bitmap->height; ly++){
        uint32_t yoff = ly*bitmap->width;
        uint32_t byoff = (y + ly) * window->width;
        for(uint32_t lx = 0; lx < bitmap->width; lx++){
            uint32_t color = bitmap->bitmap[lx+yoff];
            if(!(color & 0xFF000000)) continue;
            window->backbuffer[byoff + (x + lx)] = color;
        }
    }
}
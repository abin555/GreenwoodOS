#include "bitmap.h"
#include "allocator.h"
#include "desktop_shared.h"
#include "vfs.h"
#include "window.h"

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
    int file = vfs_open(filename, VFS_FLAG_READ);
    if(file == -1) return bitmap;
    int size = vfs_seek(file, 0, 2);
    vfs_seek(file, 0, 0);

    bitmap.file = malloc(size+100);
    vfs_read(file, (char *) bitmap.file, size);
    vfs_close(file);
    tga_header_t *header = ((tga_header_t *) bitmap.file);
    bitmap.width = header->w;
    bitmap.height = header->h;
    bitmap.bitmap = (uint32_t *) (bitmap.file + sizeof(tga_header_t) + header->magic1);
    print_serial("[DESKTOP] Loaded Icon %s - W: %d H: %d\n", filename, bitmap.width, bitmap.height);
    
    return bitmap;
}

struct Bitmap loadBigBitmap(char *filename, int max_x, int max_y){
    struct Bitmap bitmap = {
        NULL,
        NULL,
        0,
        0
    };
    int file = vfs_open(filename, VFS_FLAG_READ);
    if(file == -1) return bitmap;
    int size = vfs_seek(file, 0, 2);
    vfs_seek(file, 0, 0);

    bitmap.file = (uint8_t *) MEM_reserveRegionBlock(MEM_findRegionIdx((max_x * max_y * sizeof(uint32_t)) + sizeof(tga_header_t) + 100), (max_x * max_y * sizeof(uint32_t)) + sizeof(tga_header_t) + 100, 0, SYSTEM);
    vfs_read(file, (char *) bitmap.file, size);
    vfs_close(file);
    tga_header_t *header = ((tga_header_t *) bitmap.file);
    bitmap.width = header->w;
    bitmap.height = header->h;
    bitmap.bitmap = (uint32_t *) (bitmap.file + sizeof(tga_header_t) + header->magic1);
    print_serial("[DESKTOP] Loaded Icon %s - W: %d H: %d\n", filename, bitmap.width, bitmap.height);
    
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
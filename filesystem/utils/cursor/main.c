#include <stdint.h>
#include <stdio.h>
#include <sys/io.h>
#include <sys/memory.h>

#ifndef NULL
#define NULL 0
#endif

struct Bitmap{
    uint8_t *file;
    uint32_t *bitmap;
    uint32_t width;
    uint32_t height;
};

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

int main(int argc, char **argv){
    if(argc != 2 && argv != NULL){
        return 1;
    }
    puts("Setting Cursor!\n");

    int cursorBM_fd = open("/-/sys/cursorBM\0", O_READ | O_WRITE);
    int image_fd = open(argv[1], O_READ);
    if(cursorBM_fd == -1 || image_fd == -1){
        return 1;
    }

    struct Bitmap cursorBM;

    read(cursorBM_fd, &cursorBM, sizeof(cursorBM));

    int imageSize = lseek(image_fd, 0, 2);
    lseek(image_fd, 0, 0);

    cursorBM.file = memory_requestRegion(imageSize);

    read(image_fd, cursorBM.file, imageSize);
    tga_header_t *tga = (tga_header_t*) cursorBM.file;
    cursorBM.width = tga->w;
    cursorBM.height = tga->h;
    cursorBM.bitmap = (uint32_t *) (cursorBM.file + sizeof(tga_header_t) + tga->magic1);
    lseek(cursorBM_fd, 0, 0);
    write(cursorBM_fd, &cursorBM, sizeof(cursorBM));

    close(cursorBM_fd);
    close(image_fd);
}
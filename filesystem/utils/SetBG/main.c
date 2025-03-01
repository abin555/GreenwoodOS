#include "libc.h"

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
    if(argc != 2) return 1;
    
    int desktop_bg_fd = open("/-/sys/desktopBG", O_READ | O_WRITE);
    int image_fd = open(argv[1], O_READ);
    if(desktop_bg_fd == -1 || image_fd == -1){
        return 1;
    }

    struct Bitmap desktopBG;

    read(desktop_bg_fd, &desktopBG, sizeof(desktopBG));

    int imageSize = lseek(image_fd, 0, 2);
    lseek(image_fd, 0, 0);

    read(image_fd, desktopBG.file, imageSize);
    tga_header_t *tga = (tga_header_t*) desktopBG.file;
    desktopBG.width = tga->w;
    desktopBG.height = tga->h;
    lseek(desktop_bg_fd, 0, 0);
    write(desktop_bg_fd, &desktopBG, sizeof(desktopBG));

    close(desktop_bg_fd);
    close(image_fd);
}
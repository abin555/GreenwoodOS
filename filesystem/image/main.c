#include "libc.h"

struct WINDOW *win;

char *heap;
int heap_idx = 0;
void *alloc(int size);

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
    if(argc < 2){
        print("Not enough arguments!\n");
        return 1;
    }
    print("Opening Image Viewer\n");
    heap = (char *) 0x4000;
    
    
    set_schedule(ONFOCUS);
    print_arg("Opening Image %s\n", (uint32_t) argv[1]);
    struct FILE *image = fopen(argv[1]);
    if(image == NULL){
        print("Image Does Not Exist\n");
        return 1;
    }
    int size = fsize(image);
    print_arg("Image File Size is %d\n", size);
    char *file_buf = alloc(size);
    fcopy(image, file_buf, size);

    tga_header_t *header = (tga_header_t *) file_buf;
    print_arg("Image Width is %d\n", header->w);
    print_arg("Image Height is %d\n", header->h);

    struct WINDOW *window = window_open("Image Viewer");
    uint32_t *buffer = window->backbuffer;

    uint32_t *image_buf = (uint32_t *) (file_buf + sizeof(tga_header_t));
    int i = 0;
    for(int y = 0; y <= header->h; y++){
        for(int x = 0; x <= header->w; x++){
            buffer[y*window->width + x] = image_buf[x+y*header->w];
        }
    }
    window_update();
    while(1){

    }

    fclose(image);
    window_close(window);
    return 0;
}

void *alloc(int size){
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}
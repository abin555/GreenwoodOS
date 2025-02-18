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

struct ViewportFunctions *vp_funcs;
struct Viewport *vp;
int running = 1;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

int main(int argc, char **argv){
    if(argc < 2){
        print("Not enough arguments!\n");
        return 1;
    }
    print("Opening Image Viewer\n");
    heap = (char *) 0x4000;    
    
    print_arg("Opening Image %s\n", (uint32_t) argv[1]);
    int image = open(argv[1]);
    if(image == -1){
        print("Image Does Not Exist\n");
        return 1;
    }
    int size = lseek(image, 0, 2);
    lseek(image, 0, 0);
    print_arg("Image File Size is %d\n", size);
    char *file_buf = alloc(size);
    read(image, file_buf, size);
    close(image);

    tga_header_t *header = (tga_header_t *) file_buf;
    print_arg("Image Width is %d\n", header->w);
    print_arg("Image Height is %d\n", header->h);

    if(header->w >= 800 || header->h >= 600){
        struct WINDOW *window = window_open("Image Viewer", 0);
        uint32_t *buffer = window->backbuffer;

        uint32_t *image_buf = (uint32_t *) (file_buf + sizeof(tga_header_t));
        int i = 0;
        for(int y = 0; y < header->h; y++){
            for(int x = 0; x < header->w; x++){
                buffer[y*window->width + x] = image_buf[x+y*header->w];
            }
        }
        window_update();
        set_schedule(ONFOCUS);
        while(1){

        }
    }
    else{
        vp_funcs = viewport_get_funcs();
        vp = vp_funcs->open(header->w, header->h, argv[1]);

        uint32_t *image_buf = (uint32_t *) (file_buf + sizeof(tga_header_t));

        vp_funcs->set_buffer(vp, image_buf, header->w * header->h * sizeof(uint32_t));
        vp_funcs->add_event_handler(vp, event_handler);
        vp_funcs->copy(vp);

        set_schedule(NEVER);

        while(running){

        }
        vp_funcs->close(vp);
        print("Image Viewer Exiting\n");
    }
    return 0;
}

void *alloc(int size){
	void *address = (void *) &heap[heap_idx];
	heap_idx += size;
	return address;
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_EXIT){
        running = 0;
        set_schedule(ALWAYS);
    }
}
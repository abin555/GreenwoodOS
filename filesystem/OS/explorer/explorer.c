#include "libc.h"

int running;

struct ViewportFunctions *vp_funcs;
struct Viewport *vp;
uint32_t *buf;
#define WIDTH 500
#define HEIGHT 300

struct Bitmap{
    uint8_t *file;
    uint32_t *bitmap;
    uint32_t width;
    uint32_t height;
};

struct Bitmap folder;
struct Bitmap file;

void setup_alloc();
void *alloc(int size);
void clean_alloc();

struct Bitmap loadBitmap(char *filename);
void drawBitmap(int x, int y, struct Bitmap *bitmap);
void explorer_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

struct Vec2 {
    int x;
    int y;
};

struct Vec2 getRelativeMouse();
struct MouseStatus *getMousePtr();
struct MouseStatus *mouseStatus;

void drawIcon(int x, int y, struct DirectoryEntry *entry, int selected);
void drawDirectoryContents(struct DirectoryListing dirs, int selected);

int main(int argc, char **argv){
    setup_alloc();
    vp_funcs = viewport_get_funcs();
	vp = vp_funcs->open(WIDTH, HEIGHT, "Explorer");
	vp_funcs->add_event_handler(vp, explorer_event);
    buf = alloc(WIDTH * HEIGHT * sizeof(uint32_t));
    vp_funcs->set_buffer(vp, buf, WIDTH * HEIGHT * sizeof(uint32_t));
    mouseStatus = getMousePtr();

    folder = loadBitmap("/A/OS/icons/folder.tga");
    file = loadBitmap("/A/OS/icons/file.tga");

    struct DirectoryListing dir = getDirectoryListing(".");

    running = 1;
    int selection = 2;
    int drawAgain = 0;

    drawDirectoryContents(dir, selection);
    vp_funcs->copy(vp);

    char *work_buf = alloc(0x100);
    
    struct Vec2 relMousePos;
    print("Explorer Open\n");
	while(running){
        relMousePos = getRelativeMouse();
        if(relMousePos.x > 0 && relMousePos.x < WIDTH && relMousePos.y > 0 && relMousePos.y < HEIGHT){
            buf[relMousePos.x + relMousePos.y*WIDTH] = 0xFF00FF;
        }
        
        if(vp->ascii){
            char c = vp->ascii;
            vp->ascii = 0;
            switch(c){
                case 0x33:
                    selection = -1;
                    drawAgain = 1;
                    break;
                case 0x11:
                    selection--;
                    if(selection==-1) selection = dir.num_entries - 2;
                    drawAgain = 1;
                    break;
                case 0x12:
                    selection++;
                    if(selection==dir.num_entries-2) selection = 0;
                    drawAgain = 1;
                    break;
                case 10:{
                    if(!(selection != -1 && selection < dir.num_entries-2)) break;
                    if(dir.entries[selection+2].type == 1){
                        char *dir_str = getDirectory();
                        print_arg("Path: %s\n", (uint32_t) dir_str);
                        int i = 0;
                        for(i = 0; i < 0x100; i++) work_buf[i] = 0;
                        work_buf[0] = '/';
                        for(i = 0; dir_str[i] != '\0'; i++){
                            work_buf[i+1] = dir_str[i];
                        }
                        int walker = i+1;

                        for(i = 0; i < dir.entries[selection+2].name_len; i++){
                            work_buf[walker + i] = dir.entries[selection+2].filename[i];
                        }

                        print_arg("Changing Path to %s\n", (uint32_t) work_buf);

                        changeDirectory(work_buf);
                        dir = getDirectoryListing(".");
                        drawAgain = 1;
                        selection = -1;
                        break;
                    }
                    else if(dir.entries[selection+2].type == 0){
                        print_arg("Opening %s\n", (uint32_t) dir.entries[selection+2].filename);
                        print_arg("Checking extension: %s\n", (uint32_t) dir.entries[selection+2].filename+dir.entries[selection+2].name_len-4);
                        if(!strcmp(dir.entries[selection+2].filename+dir.entries[selection+2].name_len-4, ".tga")){
                            char **arg = alloc(sizeof(char *) * 2);
                            arg[0] = 0x0;
                            arg[1] = alloc(dir.entries[selection+2].name_len);
                            for(int i = 0; i < dir.entries[selection+2].name_len; i++) arg[1][i] = dir.entries[selection+2].filename[i];
                            exec("/A/utils/image/image.exe", 2, arg);
                            break;
                        }
                        else if(!strcmp(dir.entries[selection+2].filename+dir.entries[selection+2].name_len-4, ".exe")){
                            exec(dir.entries[selection+2].filename, 0, NULL);
                            break;
                        }
                        else if(!strcmp(dir.entries[selection+2].filename+dir.entries[selection+2].name_len-4, ".obj")){
                            char **arg = alloc(sizeof(char *) * 2);
                            arg[0] = 0x0;
                            arg[1] = alloc(dir.entries[selection+2].name_len);
                            for(int i = 0; i < dir.entries[selection+2].name_len; i++) arg[1][i] = dir.entries[selection+2].filename[i];
                            exec("/A/utils/3D/3Dvp.exe", 2, arg);
                            break;
                        }
                        else if(!strcmp(dir.entries[selection+2].filename+dir.entries[selection+2].name_len-4, ".gif")){
                            char **arg = alloc(sizeof(char *) * 2);
                            arg[0] = 0x0;
                            arg[1] = alloc(dir.entries[selection+2].name_len);
                            for(int i = 0; i < dir.entries[selection+2].name_len; i++) arg[1][i] = dir.entries[selection+2].filename[i];
                            exec("/A/utils/gif/gif.exe", 2, arg);
                            break;
                        }
                        else {
                            char **arg = alloc(sizeof(char *) * 2);
                            arg[0] = 0x0;
                            arg[1] = alloc(dir.entries[selection+2].name_len);
                            for(int i = 0; i < dir.entries[selection+2].name_len; i++) arg[1][i] = dir.entries[selection+2].filename[i];
                            exec("/A/utils/ed/ed.exe", 2, arg);
                            break;
                        }
                    }
                    break;
                }
                case 8:
                changeDirectory("..");
                dir = getDirectoryListing(".");
                drawAgain = 1;
                break;
            }
        }

        if(drawAgain){
            for(int i = 0; i < WIDTH * HEIGHT; i++) buf[i] = 0;
            drawDirectoryContents(dir, selection);
            drawAgain = 0;
        }

        vp_funcs->copy(vp);
	}
    print("Explorer Close\n");
    clean_alloc();
}

void explorer_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
	if(event == VP_MAXIMIZE || event == VP_FOCUSED){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE || event == VP_UNFOCUSED){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
    }
}

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
    struct Bitmap bitmap = {0};
    if(!fexists(filename)) return bitmap;
    struct FILE *bitmap_file = fopen(filename);
    int bitmap_size = fsize(bitmap_file);
    bitmap.file = alloc(bitmap_size);
    fcopy(bitmap_file, bitmap.file, bitmap_size);
    fclose(bitmap_file);
    tga_header_t *header = ((tga_header_t *) bitmap.file);
    bitmap.width = header->w;
    bitmap.height = header->h;
    bitmap.bitmap = (uint32_t *) (bitmap.file + sizeof(tga_header_t) + header->magic1);
    return bitmap;
}

void drawBitmap(int x, int y, struct Bitmap *bitmap){
    if(bitmap->bitmap == NULL) return;
    for(uint32_t ly = 0; ly < bitmap->height; ly++){
        uint32_t yoff = ly*bitmap->width;
        uint32_t byoff = (y + ly) * WIDTH;
        for(uint32_t lx = 0; lx < bitmap->width; lx++){
            uint32_t color = bitmap->bitmap[lx+yoff];
            if(!(color & 0xFF000000)) continue;
            buf[byoff + (x + lx)] = color;
        }
    }
}


void *alloc_base;
uint32_t alloc_size;
void *alloc_mover;

void setup_alloc(){
    alloc_size = 0x40000;
    alloc_base = requestRegion(alloc_size);
    alloc_mover = alloc_base;
}
void *alloc(int size){
    void *addr = alloc_mover;
    alloc_mover += size+1;
    for(int i = 0; i < size+1;i++){
        *(char *) addr = '\0';
    }
    return addr;
}
void clean_alloc(){
    freeRegion(alloc_base, alloc_size);
}

struct Vec2 getRelativeMouse(){
    struct Vec2 pos;
    pos.x = mouseStatus->pos.x - vp->loc.x;
    pos.y = mouseStatus->pos.y - vp->loc.y - 8;
    return pos;
}

struct MouseStatus *getMousePtr(){
	struct MouseStatus *mousePtr;
	register uint32_t eax asm("eax");
	eax = 0x27;
	asm("int 0x80");
	mousePtr = (struct MouseStatus *) eax;
	return mousePtr;
}

void drawIcon(int x, int y, struct DirectoryEntry *entry, int selected){
    if(entry == NULL) return;
    struct Bitmap *bitmap;
    if(entry->type == 0) bitmap = &file;
    else if(entry->type == 1) bitmap = &folder;
    
    drawBitmap(x+8, y, bitmap);
    int charY = y+24;
    for(int i = 0; i < entry->name_len; i++){
        if(i % 5 == 0) charY += 8;
        vp_funcs->drawChar(vp, x+((i%5)*8), charY, entry->filename[i], selected ? 0xFFFFFF : 0, selected ? 0 : 0xFFFFFF);
    }
}

void drawDirectoryContents(struct DirectoryListing dirs, int selected){
    int iconY = 0;
    int iconX = 0;
    for(int i = 0; i < dirs.num_entries-2; i++){
        drawIcon(6*8*iconX, iconY*(24+8*4), &dirs.entries[i+2], i == selected ? 1 : 0);
        iconX++;
        if(iconX >= 10){
            iconX = 0;
            iconY++;
        }
    }
}
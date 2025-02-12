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

struct RightClickMenu {
    int isVisible;
    int x;
    int y;
};

struct Vec2 getRelativeMouse();
struct MouseStatus *getMousePtr();
struct MouseStatus *mouseStatus;

void drawIcon(int x, int y, struct DirectoryEntry *entry, int selected);
void drawDirectoryContents(struct DirectoryListing *dirs, int selected, struct RightClickMenu *rightClickMenu);
int identify_selection(struct DirectoryListing *dirs);
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
void HandleSelection(char *work_buf, int sel);

struct DirectoryListing dir;
struct Vec2 relMousePos;
int selection;
int isFocus;

void drawRightClickMenu(struct RightClickMenu *rightClickMenu);
int handleRightClickMenu(struct RightClickMenu *rightClickMenu);

int main(int argc, char **argv){
    setup_alloc();
    vp_funcs = viewport_get_funcs();
	vp = vp_funcs->open(WIDTH, HEIGHT, "Explorer");
    isFocus = 1;
	vp_funcs->add_event_handler(vp, explorer_event);
    buf = alloc(WIDTH * HEIGHT * sizeof(uint32_t));
    vp_funcs->set_buffer(vp, buf, WIDTH * HEIGHT * sizeof(uint32_t));
    mouseStatus = getMousePtr();

    folder = loadBitmap("/A/OS/icons/folder.tga");
    file = loadBitmap("/A/OS/icons/file.tga");

    dir = getDirectoryListing(".");

    running = 1;
    selection = 2;
    int last_mouse_left = 0;
    int last_mouse_right = 0;
    struct RightClickMenu rightClickMenu = {0, 0, 0};

    for(int i = 0; i < WIDTH * HEIGHT; i++) buf[i] = 0;
    drawDirectoryContents(&dir, selection, &rightClickMenu);
    vp_funcs->copy(vp);

    char *work_buf = alloc(0x100);
    
    print("Explorer Open\n");
	while(running){
        relMousePos = getRelativeMouse();
        if(relMousePos.x > 0 && relMousePos.x < WIDTH && relMousePos.y > 0 && relMousePos.y < HEIGHT){
            buf[relMousePos.x + relMousePos.y*WIDTH] = 0xFF00FF;
        }
        selection = identify_selection(&dir);
        int temp;
        
        if(vp->ascii){
            char c = vp->ascii;
            vp->ascii = 0;
            switch(c){
                case 0x33:
                    selection = -1;
                    break;
                case 0x11:
                    selection--;
                    if(selection==-1) selection = dir.num_entries - 2;
                    break;
                case 0x12:
                    selection++;
                    if(selection==dir.num_entries-2) selection = 0;
                    break;
                case 10:{
                    temp = selection;
                    task_lock(1);
                    HandleSelection(work_buf, temp);
                    selection = -1;
                    task_lock(0);
                    break;
                }
                case 8:
                changeDirectory("..");
                dir = getDirectoryListing(".");
                break;
            }
        }
        if(mouseStatus->buttons.left && !last_mouse_left && isFocus){
            last_mouse_left = 1;
            if(selection != -1 && !rightClickMenu.isVisible){
                temp = selection;
                selection = -1;
                task_lock(1);
                HandleSelection(work_buf, temp);
                selection = -1;
                task_lock(0);
            }
            if(rightClickMenu.isVisible && !handleRightClickMenu(&rightClickMenu)){
                rightClickMenu.isVisible = 0;
                dir = getDirectoryListing(".");
            }
        }
        if(!mouseStatus->buttons.left && last_mouse_left){
            last_mouse_left = 0;
        }

        if(mouseStatus->buttons.right && !last_mouse_right && isFocus){
            last_mouse_right = 1;
            rightClickMenu.isVisible = 1;
            rightClickMenu.x = mouseStatus->pos.x - vp->loc.x;
            rightClickMenu.y = mouseStatus->pos.y - vp->loc.y;

        }
        if(!mouseStatus->buttons.right && last_mouse_right){
            last_mouse_right = 0;
            //rightClickMenu.isVisible = 0;
        }

        for(int i = 0; i < WIDTH * HEIGHT; i++) buf[i] = 0;
        drawDirectoryContents(&dir, selection, &rightClickMenu);
        if(rightClickMenu.isVisible){
            drawRightClickMenu(&rightClickMenu);
        }

        vp_funcs->copy(vp);
	}
    print("Explorer Close\n");
    clean_alloc();
}

void HandleSelection(char *work_buf, int sel){
    char *application_string = NULL;
    char **arg = alloc(sizeof(char *) * 2);

    if(!(sel != -1 && sel < dir.num_entries-2)) return;
    if(dir.entries[sel+2].type == 1){
        char *dir_str = getDirectory();
        print_arg("Path: %s\n", (uint32_t) dir_str);
        int i = 0;
        for(i = 0; i < 0x100; i++) work_buf[i] = 0;
        work_buf[0] = '/';
        for(i = 0; dir_str[i] != '\0'; i++){
            work_buf[i+1] = dir_str[i];
        }
        int walker = i+1;

        for(i = 0; i < dir.entries[sel+2].name_len; i++){
            work_buf[walker + i] = dir.entries[sel+2].filename[i];
        }

        print_arg("Changing Path to %s\n", (uint32_t) work_buf);

        changeDirectory(work_buf);
        dir = getDirectoryListing(".");
        sel = -1;
        return;
    }

    else if(dir.entries[sel+2].type == 0){
        print_arg("Opening %s\n", (uint32_t) dir.entries[sel+2].filename);
        print_arg("Checking extension: %s\n", (uint32_t) dir.entries[sel+2].filename+dir.entries[sel+2].name_len-4);
        if(!strcmp(dir.entries[sel+2].filename+dir.entries[sel+2].name_len-4, ".tga")){
            
            arg[0] = 0x0;
            arg[1] = alloc(dir.entries[sel+2].name_len);
            for(int i = 0; i < dir.entries[sel+2].name_len; i++) arg[1][i] = dir.entries[sel+2].filename[i];
            sel = -1;
            exec("/A/utils/image/image.exe", 2, arg);
            return;
        }
        else if(!strcmp(dir.entries[sel+2].filename+dir.entries[sel+2].name_len-4, ".exe")){
            exec(dir.entries[sel+2].filename, 0, NULL);
            sel = -1;
            return;
        }
        else if(!strcmp(dir.entries[sel+2].filename+dir.entries[sel+2].name_len-4, ".obj")){
            char **arg = alloc(sizeof(char *) * 2);
            arg[0] = 0x0;
            arg[1] = alloc(dir.entries[sel+2].name_len);
            for(int i = 0; i < dir.entries[sel+2].name_len; i++) arg[1][i] = dir.entries[sel+2].filename[i];
            sel = -1;
            exec("/A/utils/3D/3Dvp.exe", 2, arg);
            return;
        }
        else if(!strcmp(dir.entries[sel+2].filename+dir.entries[sel+2].name_len-4, ".gif")){
            char **arg = alloc(sizeof(char *) * 2);
            arg[0] = 0x0;
            arg[1] = alloc(dir.entries[sel+2].name_len);
            for(int i = 0; i < dir.entries[sel+2].name_len; i++) arg[1][i] = dir.entries[sel+2].filename[i];
            sel = -1;
            exec("/A/utils/gif/gif.exe", 2, arg);
            return;
        }
        else {
            char **arg = alloc(sizeof(char *) * 2);
            arg[0] = 0x0;
            arg[1] = alloc(dir.entries[sel+2].name_len);
            for(int i = 0; i < dir.entries[sel+2].name_len; i++) arg[1][i] = dir.entries[sel+2].filename[i];
            sel = -1;
            exec("/A/utils/ed/ed.exe", 2, arg);
            return;
        }
    }
    
    if(application_string != NULL && arg != NULL){
        exec(application_string, 2, arg);
    }    
}

void explorer_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
	if(event == VP_MAXIMIZE/* || event == VP_FOCUSED */){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE/* || event == VP_UNFOCUSED */){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
    }
    else if(event == VP_FOCUSED){
        isFocus = 1;
        set_schedule(ALWAYS);
    }
    else if(event == VP_UNFOCUSED){
        isFocus = 0;
        set_schedule(NEVER);
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

int identify_selection(struct DirectoryListing *dirs){
    int iconY = 0;
    int iconX = 0;
    int found_selection = 0;
    for(int i = 0; i < dirs->num_entries-2; i++){
        if(
            relMousePos.x > 6*8*iconX && relMousePos.x < (6*8*iconX + 4*8) &&
            relMousePos.y > iconY*(24+8*4) && relMousePos.y < (iconY*(24+8*4) + 4*8) &&
            isFocus
        ){
            return i;
        }
        iconX++;
        if(iconX >= 10){
            iconX = 0;
            iconY++;
        }
    }
    return -1;
}

void drawDirectoryContents(struct DirectoryListing *dirs, int selected, struct RightClickMenu *rightClickMenu){
    int iconY = 0;
    int iconX = 0;
    for(int i = 0; i < dirs->num_entries-2; i++){
        drawIcon(6*8*iconX, iconY*(24+8*4), &dirs->entries[i+2], i == selected ? 1 : 0);
        if(
            relMousePos.x > 6*8*iconX && relMousePos.x < (6*8*iconX + 4*8) &&
            relMousePos.y > iconY*(24+8*4) && relMousePos.y < (iconY*(24+8*4) + 4*8) &&
            isFocus &&
            !rightClickMenu->isVisible
        ){
            drawRect(
                0x0000FF, 0x000033,
                6*8*iconX, iconY*(24+8*4),
                6*8*iconX+4*8, iconY*(24+8*4)+4*8,
                buf, WIDTH
            );
        }
        iconX++;
        if(iconX >= 10){
            iconX = 0;
            iconY++;
        }
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

void drawRightClickMenu(struct RightClickMenu *rightClickMenu){
    drawRect(
        0x5c5c5c,
        0xbfbfbf,
        rightClickMenu->x-2,
        rightClickMenu->y-2,
        rightClickMenu->x+(8*10)+4,
        rightClickMenu->y+(2*8)+4,
        buf,
        WIDTH
    );
    static char newFile[] = "New File";
    static char newFolder[] = "New Folder";
    int mouse_entry_hover = -1;
    if(
        mouseStatus->pos.x - vp->loc.x >= rightClickMenu->x-2 &&
        mouseStatus->pos.x - vp->loc.x <= rightClickMenu->x+(8*10)+4 &&
        mouseStatus->pos.y - vp->loc.y >= rightClickMenu->y-2 &&
        mouseStatus->pos.y - vp->loc.y <= rightClickMenu->y+(2*8)+4
    ){
        mouse_entry_hover = ((mouseStatus->pos.y - vp->loc.y - 8 - rightClickMenu->y+1) / 8) + 1;
        if(mouse_entry_hover != 1 && mouse_entry_hover != 2) mouse_entry_hover = -1;
    }
    for(int i = 0; i < sizeof(newFile)-1; i++){
        vp_funcs->drawChar(
            vp, rightClickMenu->x+(8*i)+1, rightClickMenu->y+0+1, newFile[i], mouse_entry_hover == 1 ? 0xFFFFFF : 0x0, mouse_entry_hover == 1 ? 0x0 : 0xbfbfbf
        );
    }
    for(int i = 0; i < sizeof(newFolder)-1; i++){
        vp_funcs->drawChar(
            vp, rightClickMenu->x+(8*i)+1, rightClickMenu->y+8+1, newFolder[i], mouse_entry_hover == 2 ? 0xFFFFFF : 0x0, mouse_entry_hover == 2 ? 0x0 : 0xbfbfbf
        );
    }
}

void memset(void* ptr, int value, int num)
{
    unsigned char* p = ptr;
    for (int i = 0; i < num; ++i, ++p)
    {
        *p = (unsigned char)value;
    }
}

void popupFilename(char *filebuf, int filebuf_size){
    if(filebuf == NULL || filebuf_size == 0) return;
    print_serial("Popup Filename Search\n");
    int mover = 0;
    int getting_filename = 1;
    filebuf[0] = 'A';
    while(getting_filename){
        drawRect(
            0x5c5c5c,
            0xbfbfbf,
            (vp->loc.w / 2)-(filebuf_size*8 / 2)-2,
            (vp->loc.h / 2)-(6),
            (vp->loc.w / 2)+(filebuf_size*8 / 2)+2,
            (vp->loc.h / 2)+(6),
            buf,
            WIDTH
        );
        for(int i = 0; i < filebuf_size && filebuf[i] != 0; i++){
            vp_funcs->drawChar(
                vp,
                (vp->loc.w / 2)-(filebuf_size*8 / 2)+(i*8),
                (vp->loc.h / 2)-(4),
                filebuf[i],
                0x0,
                0xbfbfbf
            );
        }
        vp_funcs->copy(vp);

        char c = vp_funcs->getc(vp);
        if(c != 0){
            print_arg("Got %c\n", c);
            print_serial("Print Progress:");
            print_serial(filebuf);
            print_serial("\n");
            switch(c){
                case 10:
                    getting_filename = 0;
                    break;
                case 8:
                    mover--;
                    if(mover < 0) mover = 0;
                    filebuf[mover] = 0;
                    break;
                default:
                    filebuf[mover++] = c;
                    break;
            }
        }
        vp_funcs->copy(vp);
    }
}

int handleRightClickMenu(struct RightClickMenu *rightClickMenu){
    int mouse_entry_hover = -1;
    if(
        mouseStatus->pos.x - vp->loc.x >= rightClickMenu->x-2 &&
        mouseStatus->pos.x - vp->loc.x <= rightClickMenu->x+(8*10)+4 &&
        mouseStatus->pos.y - vp->loc.y >= rightClickMenu->y-2 &&
        mouseStatus->pos.y - vp->loc.y <= rightClickMenu->y+(2*8)+4
    ){
        mouse_entry_hover = ((mouseStatus->pos.y - vp->loc.y - 8 - rightClickMenu->y+1) / 8) + 1;
        if(mouse_entry_hover != 1 && mouse_entry_hover != 2) mouse_entry_hover = -1;
    }
    static char filename_buf[50];
    memset(filename_buf, 0, sizeof(filename_buf));

    if(mouse_entry_hover == -1){
        return 0;
    }

    popupFilename(filename_buf, sizeof(filename_buf));

    if(mouse_entry_hover == 1){
        fmkfile(filename_buf, 0x1000);
    }
    if(mouse_entry_hover == 2){
        fmkdir(filename_buf);
    }

    return 0;
}
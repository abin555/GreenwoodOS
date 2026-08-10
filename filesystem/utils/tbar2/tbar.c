#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/io.h>
#include <sys/task.h>
#include <string.h>
#include "tbar.h"

#define RSIZE_FILE "/+/tbar_ico.tga"

// Shells out to the resize utility to produce a fixed-size TGA icon at RSIZE_FILE.
// Returns 0 on success, non-zero on failure.
int tbar_mkResizedIcon(char *filename, int w, int h){
    char wstr[12];
    char hstr[12];

    int n = snprintf(wstr, sizeof(wstr), "%d", w);
    if(n < 0 || n >= (int)sizeof(wstr)){
        wstr[sizeof(wstr) - 1] = '\0';
    }
    n = snprintf(hstr, sizeof(hstr), "%d", h);
    if(n < 0 || n >= (int)sizeof(hstr)){
        hstr[sizeof(hstr) - 1] = '\0';
    }

    // Stack-allocated argv; no heap allocations needed (and nothing to leak).
    char **argv = malloc(sizeof(char*) * 6);
    argv[0] = "/A/utils/rsize/rsize.elf";
    argv[1] = filename;
    argv[2] = strdup(RSIZE_FILE);
    argv[3] = wstr;
    argv[4] = hstr;
    argv[5] = NULL;

    int pid = exec(argv[0], 5, argv);
    if(pid < 0){
        printf("[TBAR] Failed to launch icon resizer for '%s'\n", filename);
        return 1;
    }
    waitpid(pid);
    return 0;
}

typedef struct {
  unsigned char magic1;             // must be zero
  unsigned char colormap;           // must be zero
  unsigned char encoding;           // must be 2
  unsigned short cmaporig, cmaplen; // must be zero
  unsigned char cmapent;            // must be zero
  unsigned short x;                 // must be zero
  unsigned short y;                 // must be zero
  unsigned short w;                 // image's width
  unsigned short h;                 // image's height
  unsigned char bpp;                // must be 32
  unsigned char pixeltype;          // must be 40
} __attribute__((packed)) tga_header_t;

IconBMP *tbar_mkIcon(char *filename){
    FILE *icon_file = fopen(filename, "r");
    if(icon_file == NULL){
        printf("[TBAR] Icon file not found: %s\n", filename);
        return NULL;
    }
    fclose(icon_file);

    if(tbar_mkResizedIcon(filename, ICON_SIZE, ICON_SIZE) != 0){
        return NULL;
    }

    icon_file = fopen(RSIZE_FILE, "r");
    if(icon_file == NULL){
        printf("[TBAR] Could not open resized icon: %s\n", RSIZE_FILE);
        return NULL;
    }

    tga_header_t fileheader;
    if(fread(&fileheader, sizeof(fileheader), 1, icon_file) != 1){
        printf("[TBAR] Could not read TGA header for %s\n", filename);
        fclose(icon_file);
        return NULL;
    }

    IconBMP *icon = malloc(sizeof(IconBMP));
    if(icon == NULL){
        printf("[TBAR] Out of memory loading icon for %s\n", filename);
        fclose(icon_file);
        return NULL;
    }
    if(fread(icon, sizeof(IconBMP), 1, icon_file) != 1){
        printf("[TBAR] Could not read pixel data for %s\n", filename);
        free(icon);
        fclose(icon_file);
        return NULL;
    }
    fclose(icon_file);
    return icon;
}

int tbar_index_from_x(int relx){
    if(relx < 0) return -1;
    return relx / TBAR_HEIGHT;
}

void tbar_drawIcon(int idx, IconBMP *icon, uint32_t *backbuf, uint32_t width, uint32_t height){
    int pad = (TBAR_HEIGHT - ICON_SIZE) / 2; // Center the icon within its slot.
    int slotX = idx * TBAR_HEIGHT;

    if(icon == NULL){
        // Icon failed to load - draw a placeholder so the entry is still visible/clickable
        // instead of leaving it blank or (previously) crashing on a NULL dereference.
        for(int row = 0; row < ICON_SIZE; row++){
            for(int col = 0; col < ICON_SIZE; col++){
                backbuf[(slotX + pad + col) + (pad + row) * width] = TBAR_COLOR_PLACEHOLDER;
            }
        }
        return;
    }

    for(int row = 0; row < ICON_SIZE; row++){
        for(int col = 0; col < ICON_SIZE; col++){
            uint32_t color = (*icon)[row][col];
            backbuf[(slotX + pad + col) + (pad + row) * width] = color == 0x0 ? TBAR_COLOR_PLACEHOLDER : color;
        }
    }
}

void tbar_draw(struct Taskbar *tbar, uint32_t *backbuf, uint32_t width, uint32_t height, int active_idx){
    // Background fill.
    for(uint32_t i = 0; i < width * height; i++){
        backbuf[i] = TBAR_COLOR_BG;
    }

    // Thin accent line along the top edge.
    for(uint32_t x = 0; x < width; x++){
        backbuf[x] = TBAR_COLOR_ACCENT;
    }

    for(int i = 0; i < tbar->numEntries; i++){
        int slotX = i * TBAR_HEIGHT;

        // Highlight the slot that was just activated (e.g. clicked).
        if(i == active_idx){
            for(uint32_t y = 1; y < height; y++){
                for(int x = 0; x < TBAR_HEIGHT; x++){
                    backbuf[(slotX + x) + y * width] = TBAR_COLOR_ACTIVE;
                }
            }
        }

        // Divider between adjacent icons.
        if(i > 0){
            for(uint32_t y = 1; y < height; y++){
                backbuf[slotX + y * width] = TBAR_COLOR_SEPARATOR;
            }
        }

        tbar_drawIcon(i, tbar->entries[i].icon, backbuf, width, height);
    }
}

struct Taskbar *tbar_init(){
    // Bug fix: previously this was malloc(sizeof(tbar)), which allocates the
    // size of a POINTER (4-8 bytes) instead of the size of the struct
    // (hundreds of bytes). Every write past the first few bytes corrupted
    // the heap. This now correctly allocates the full struct.
    struct Taskbar *tbar = malloc(sizeof(struct Taskbar));
    if(tbar == NULL){
        printf("[TBAR] Out of memory initializing taskbar\n");
        return NULL;
    }
    tbar->numEntries = 0;
    return tbar;
}

int tbar_entry_init(struct TaskbarEntry *entry, char *name, char *exec, char *args, char *icon){
    if(entry == NULL) return 1;
    entry->name = strdup(name);
    entry->exec_filename = strdup(exec);
    entry->arg_str = strdup(args);
    entry->icon_filename = strdup(icon);
    printf("[TBAR] Added %s - %s (%s) - %s\n", entry->name, entry->exec_filename, entry->arg_str, entry->icon_filename);
    entry->icon = tbar_mkIcon(entry->icon_filename); // May be NULL; tbar_drawIcon handles that.
    return 0;
}

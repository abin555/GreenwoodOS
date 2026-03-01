#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <sys/io.h>
#include <sys/task.h>
#include <string.h>
#include "tbar.h"

#define RSIZE_FILE "/+/tbar_ico.tga"

int tbar_mkResizedIcon(char *filename, int w, int h){
    char wstr[10];
    int n = snprintf(wstr, sizeof(wstr), "%d\0", w);
    wstr[n] = '\0';
    char hstr[10];
    n = snprintf(hstr, sizeof(hstr), "%d\0", h);
    hstr[n] = '\0';
    char **argv = malloc(sizeof(char *) * 6);
    argv[0] = "/A/utils/rsize/rsize.elf";
    argv[1] = strdup(filename);
    argv[2] = strdup(RSIZE_FILE);
    argv[3] = strdup(wstr);
    argv[4] = strdup(hstr);
    argv[5] = NULL;
    int pid = exec(argv[0], 5, argv);
    waitpid(pid);
    printf("Icon Hopefully Resized!\n");
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

IconBMP *tbar_mkIcon(char *filename){
    FILE *icon_file = fopen(filename, "r");
    if(icon_file == NULL) return NULL;
    fclose(icon_file);
    tbar_mkResizedIcon(filename, TBAR_HEIGHT, TBAR_HEIGHT);
    icon_file = fopen(RSIZE_FILE, "r");
    if(icon_file == NULL) return NULL;


    tga_header_t fileheader;
    fread(&fileheader, sizeof(fileheader), 1, icon_file);
    IconBMP *icon = malloc(sizeof(IconBMP));
    fread(icon, sizeof(IconBMP), 1, icon_file);
    fclose(icon_file);
    return icon;
}

void tbar_drawIcon(int idx, IconBMP *icon, uint32_t *backbuf, uint32_t width, uint32_t height){
    int x = idx*TBAR_HEIGHT;
    for(int i = 0; i < TBAR_HEIGHT; i++){
        for(int j = 0; j < TBAR_HEIGHT; j++){
            backbuf[x + j + i * width] = (*icon)[i][j];
        }
    }
}

struct Taskbar *tbar_init(){
    struct Taskbar *tbar = malloc(sizeof(tbar));
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
    entry->icon = tbar_mkIcon(entry->icon_filename);
    return 0;
}
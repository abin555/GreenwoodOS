#ifndef TBAR_H
#define TBAR_H

#include <stdint.h>

#define TBAR_HEIGHT 28

typedef uint32_t IconBMP[TBAR_HEIGHT][TBAR_HEIGHT] ;

struct TaskbarEntry {
    char *name;
    char *exec_filename;
    char *arg_str;
    char *icon_filename;
    IconBMP *icon;
};

struct Taskbar {
    int numEntries;
    struct TaskbarEntry entries[20];
};

int tbar_mkResizedIcon(char *filename, int w, int h);
IconBMP *tbar_mkIcon(char *filename);
void tbar_drawIcon(int idx, IconBMP *icon, uint32_t *backbuf, uint32_t width, uint32_t height);

struct Taskbar *tbar_init();
int tbar_entry_init(struct TaskbarEntry *entry, char *name, char *exec, char *args, char *icon);
void tbar_draw(struct Taskbar *tbar, uint32_t *backbuf, uint32_t width, uint32_t height);

#endif
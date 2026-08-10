#ifndef TBAR_H
#define TBAR_H

#include <stdint.h>

#define TBAR_HEIGHT      28   // Height of the taskbar, and the width of each icon "slot".
#define ICON_SIZE        24   // Icon width/height in pixels.
#define TBAR_MAXICONS    6    // Max icons the taskbar viewport is sized to display.
#define TBAR_MAX_ENTRIES 20   // Max entries that can be loaded from a config file.

// Colors are 0x00RRGGBB.
#define TBAR_COLOR_BG          0x002B2D31 // Background
#define TBAR_COLOR_ACCENT      0x005865F2 // Top accent border
#define TBAR_COLOR_ACTIVE      0x003A3D42 // Flashed on the icon that was just clicked
#define TBAR_COLOR_SEPARATOR   0x00393B40 // Thin divider between icon slots
#define TBAR_COLOR_PLACEHOLDER 0x004E5157 // Shown when an icon fails to load

typedef uint32_t IconBMP[ICON_SIZE][ICON_SIZE];

struct TaskbarEntry {
    char *name;
    char *exec_filename;
    char *arg_str;
    char *icon_filename;
    IconBMP *icon; // NULL if the icon failed to load; a placeholder is drawn instead.
};

struct Taskbar {
    int numEntries;
    struct TaskbarEntry entries[TBAR_MAX_ENTRIES];
};

int tbar_mkResizedIcon(char *filename, int w, int h);
IconBMP *tbar_mkIcon(char *filename);
void tbar_drawIcon(int idx, IconBMP *icon, uint32_t *backbuf, uint32_t width, uint32_t height);

struct Taskbar *tbar_init();
int tbar_entry_init(struct TaskbarEntry *entry, char *name, char *exec, char *args, char *icon);

// Renders the full taskbar (background, separators, icons) into backbuf.
// active_idx is the entry to visually highlight (e.g. the one just clicked), or -1 for none.
void tbar_draw(struct Taskbar *tbar, uint32_t *backbuf, uint32_t width, uint32_t height, int active_idx);

// Converts an x coordinate relative to the taskbar viewport into an icon slot index.
// Returns -1 if relx is negative. Caller is still responsible for checking the
// result against Taskbar->numEntries.
int tbar_index_from_x(int relx);

#endif

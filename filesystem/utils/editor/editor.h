#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <sys/vp.h>
#include "gui.h"

struct EditorFile {
    char *path;
    int size;
    FILE *file;
    char *loaded_file;
    unsigned int loaded_idx;
    int growth;
    int cursorIdx;
    int mouseDown;
};

struct EditorFile editor_open(char *path);
void editor_reload(struct EditorFile *editor);
void editor_close(struct EditorFile *file);
void editor_draw(struct WindowContext *ctx, struct Location subbox, struct EditorFile *file, int lineStart, int lineEnd);
void editor_save(struct EditorFile *editor);
void editor_update(struct WindowContext *ctx, struct EditorFile *file);
unsigned int editor_getLineFromFilePct(struct EditorFile *file, double pct);
unsigned int editor_getLineCount(struct EditorFile *file);


#endif
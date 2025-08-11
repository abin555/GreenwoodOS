#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <sys/vp.h>

struct EditorFile {
    char *path;
    int size;
    FILE *file;
    char *loaded_file;
    unsigned int loaded_idx;
};

struct EditorFile editor_open(char *path);
void editor_reload(struct EditorFile *editor);
void editor_close(struct EditorFile *file);
void editor_draw(struct Viewport *vp, struct Location subbox, struct EditorFile *file, int lineStart, int lineEnd, int mouseX, int mouseY);
unsigned int editor_getLineFromFilePct(struct EditorFile *file, double pct);
unsigned int editor_getLineCount(struct EditorFile *file);


#endif
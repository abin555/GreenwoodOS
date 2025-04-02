#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "editor.h"

struct EditorFile editor_open(char *path){
    struct EditorFile file;
    file.path = strdup(path);
    file.file = fopen(file.path, "rw+");
    if(file.file == NULL) return file;

    fseek(file.file, 0, SEEK_END);
    file.size = ftell(file.file);
    fseek(file.file, 0, SEEK_SET);

    file.loaded_idx = 0;
    file.loaded_file = malloc(sizeof(char) * file.size * 2);

    fread(file.loaded_file, file.size, 1, file.file);
    fseek(file.file, 0, SEEK_SET);

    printf("Loaded %s of size %d\nHas %d lines\n", file.path, file.size, editor_getLineCount(&file));

    return file;
}

void editor_close(struct EditorFile *file){
    if(file == NULL) return;
    fclose(file->file);
    free(file->path);
    free(file->loaded_file);
}

unsigned int editor_getLineCount(struct EditorFile *file){
    if(file == NULL || file->loaded_file == NULL) return 0;
    unsigned int numLines = 0;
    for(int i = 0; i < file->size; i++){
        if(file->loaded_file[i] == '\n'){
            numLines++;
        }
    }
    return numLines;
}

unsigned int editor_getLineFromFilePct(struct EditorFile *file, double pct){
    unsigned int nLines = editor_getLineCount(file);
    unsigned int line = (unsigned int) ((double) nLines * pct);
    return line;
}

void editor_draw(struct Viewport *vp, struct Location subbox, struct EditorFile *file, int lineStart, int lineEnd){
    int startIdx;
    int endIdx;
    int lineWalks = 0;
    for(startIdx = 0; startIdx < file->size; startIdx++){
        if(file->loaded_file[startIdx] == '\n') lineWalks++;
        if(lineWalks == lineStart) break;
    }
    for(endIdx = startIdx; endIdx < file->size; endIdx++){
        if(file->loaded_file[endIdx] == '\n') lineWalks++;
        if(lineWalks == lineEnd) break;
    }

    int cX, cY = 0;
    int mX, mY;
    mX = (subbox.x + subbox.w);
    mY = (subbox.y + subbox.h);
    for(int i = startIdx; i < endIdx; i++){
        if(cX >= mX){
            cX = 0;
            cY += 8;
            while(file->loaded_file[i] != '\n'){
                i++;
            }
            i++;
        }
        if(file->loaded_file[i] == '\n'){
            cX = 0;
            cY += 8;
        }
        else{
            vp_drawChar(vp, subbox.x + cX, subbox.y + cY, file->loaded_file[i], 0xFFFFFF, 0x0);
            cX += 8;
        }
    }
}
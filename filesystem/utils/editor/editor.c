#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/memory.h>
#include <internal/stdio.h>
#include <sys/io.h>
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
    file.growth = 0;
    file.loaded_file = memory_requestRegion(sizeof(char)*file.size*2);

    fread(file.loaded_file, file.size, 1, file.file);
    fseek(file.file, 0, SEEK_SET);

    file.cursorIdx = 0;
    file.mouseDown = 0;

    printf("Loaded %s of size %d\nHas %d lines\n", file.path, file.size, editor_getLineCount(&file));

    return file;
}

void editor_reload(struct EditorFile *editor){
    if(editor == NULL) return;
    fseek(editor->file, 0, SEEK_SET);
    fread(editor->loaded_file, editor->size, 1, editor->file);
    fseek(editor->file, 0, SEEK_SET);
    editor->growth = 0;
}

void editor_save(struct EditorFile *editor){
    if(editor == NULL) return;
    printf("Saving - %d bytes (%d original, %d new)\n", editor->size + editor->growth, editor->size, editor->growth);
    ftruncate(editor->file->fd, editor->size + editor->growth);
    fseek(editor->file, 0, SEEK_SET);
    fwrite(editor->loaded_file, editor->size + editor->growth, 1, editor->file);
}

void editor_close(struct EditorFile *file){
    if(file == NULL) return;
    memory_returnRegion(file->loaded_file, sizeof(char)*file->size*2);
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

void editor_insert(struct EditorFile *file, int offset, char c){
    for(int i = file->size+file->growth; i > offset; i--){
        file->loaded_file[i] = file->loaded_file[i-1];
    }
    file->loaded_file[offset] = c;
}

void editor_update(struct WindowContext *ctx, struct EditorFile *file){
    if(ctx->viewport->ascii != 0){
        //file->loaded_file[file->cursorIdx++] = ctx->viewport->ascii;
        switch(ctx->viewport->ascii){
            case 19://Up Arrow
                break;
            case 20://Down Arrow
                break;
            case 17://Left Arrow
                file->cursorIdx--;
                break;
            case 18://Right Arrow
                file->cursorIdx++;
                break;
            case 8://Backspace
                file->cursorIdx--;
                memcpy(file->loaded_file+file->cursorIdx, file->loaded_file+file->cursorIdx+1,file->size+file->growth-file->cursorIdx);
                file->growth--;
                break;
            case 10://Enter
                editor_insert(file, file->cursorIdx++, '\n');
                file->growth++;
                break;
            default:
                editor_insert(file, file->cursorIdx++, ctx->viewport->ascii);
                file->growth++;
                break;
        }
        ctx->viewport->ascii = 0;
    }
}

void editor_draw(struct WindowContext *ctx, struct Location subbox, struct EditorFile *file, int lineStart, int lineEnd){
    int startIdx;
    int endIdx;
    int lineWalks = 0;
    for(startIdx = 0; startIdx < file->size; startIdx++){
        if(file->loaded_file[startIdx] == '\n') lineWalks++;
        if(lineWalks == lineStart) break;
    }
    for(endIdx = startIdx; endIdx < file->size + file->growth; endIdx++){
        if(file->loaded_file[endIdx] == '\n') lineWalks++;
        if(lineWalks == lineEnd) break;
    }

    int mGridX, mGridY = -1;

    if(
        ctx->localMouseX > subbox.x && ctx->localMouseX < subbox.x+subbox.w &&
        ctx->localMouseY > subbox.y && ctx->localMouseY < subbox.y+subbox.h
    ){
        mGridX = (ctx->localMouseX - subbox.x) / 8;
        mGridY = (ctx->localMouseY - subbox.y) / 8;
    }
    else{
        mGridX = -1;
        mGridY = -1;
    }

    int cX = 0;
    int cY = 8;
    int mX, mY;
    mX = (subbox.x + subbox.w);
    mY = (subbox.y + subbox.h);
    for(int i = startIdx; i < endIdx; i++){
        if(cX >= mX-1){
            cX = 0;
            cY += 8;
            while(file->loaded_file[i] != '\n' && file->loaded_file[i] != '\0'){
                i++;
            }
            i++;
        }
        if(file->loaded_file[i] == '\n'){
            if(i == file->cursorIdx){
                vp_drawChar(ctx->viewport, subbox.x + cX, subbox.y + cY, '|', 0x0, 0xFFFFFF);
            }
            cX = 0;
            cY += 8;
        }
        else{
            if(i == file->cursorIdx){
                vp_drawChar(ctx->viewport, subbox.x + cX, subbox.y + cY, file->loaded_file[i], 0x0, 0xFFFFFF);
            }
            else{
                if(((cX / 8) == mGridX) && ((cY / 8) == mGridY)){
                    vp_drawChar(ctx->viewport, subbox.x + cX, subbox.y + cY, file->loaded_file[i], 0xFFFFFF, 0x770077);
                    if(ctx->mouseStatus.buttons.left && !file->mouseDown){
                        file->cursorIdx = i;
                        printf("Mouse Click: %d\n", file->cursorIdx);
                    }
                    else if(!ctx->mouseStatus.buttons.left){
                        file->mouseDown = 0;
                    }
                }
                else{
                    vp_drawChar(ctx->viewport, subbox.x + cX, subbox.y + cY, file->loaded_file[i], 0xFFFFFF, 0x0);
                }
            }
            cX += 8;
        }
    }
}
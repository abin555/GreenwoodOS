#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/vp.h>
#include <sys/task.h>
#include "gui.h"

enum PROC_CMD {
    PROC_GET_TASK_COUNT = 0,
    PROC_GET_TASK = 1,
    PROC_PAUSE_TASK = 2,
    PROC_RESUME_TASK = 3,
};

struct PROC_Request {
    enum PROC_CMD cmd;
    int task_id;
    unsigned int command;
};

struct PROC_Response_Task_Info {
    enum PROC_CMD cmd;
    int task_id;
    int slot;
    uint8_t slot_active;
    int schedule_type;
    char *name;
    int pid;
};

struct Context {
    int ntasks;
    struct PROC_Response_Task_Info taskInfo[20];
};

void updateTaskContext(FILE *task_file, struct Context *ctx){
    struct PROC_Request req_ntasks;
    req_ntasks.cmd = PROC_GET_TASK_COUNT;
    fwrite(&req_ntasks, sizeof(req_ntasks), 1, task_file);
    int n_tasks;
    fread(&n_tasks, sizeof(n_tasks), 1, task_file);
    //printf("There are %d tasks!\n", n_tasks);
    ctx->ntasks = n_tasks;

    for(int i = 0; i < ctx->ntasks && i < 20; i++){
        struct PROC_Request req_task;
        req_task.cmd = PROC_GET_TASK;
        req_task.task_id = i;
        fwrite(&req_task, sizeof(req_task), 1, task_file);
        struct PROC_Response_Task_Info task_info;
        fread(&ctx->taskInfo[i], sizeof(task_info), 1, task_file);
        //printf("#%d - Slot: %d \"%s\"\n", i, ctx->taskInfo[i].slot, ctx->taskInfo[i].name);
    }
}

void pause_task(FILE *task_file, int tid){
    struct PROC_Request req_task;
    req_task.cmd = PROC_PAUSE_TASK;
    req_task.task_id = tid;
    fwrite(&req_task, sizeof(req_task), 1, task_file);
}

void resume_task(FILE *task_file, int tid){
    struct PROC_Request req_task;
    req_task.cmd = PROC_RESUME_TASK;
    req_task.task_id = tid;
    fwrite(&req_task, sizeof(req_task), 1, task_file);
}

#define ColorText 0x0
#define ColorBG 0xb9b9b9

int running;

char sched_to_char(ScheduleType sched){
    switch(sched){
        case ALWAYS:
            return 'A';
        case ONFOCUS:
            return 'F';
        case NOCHILD:
            return 'C';
        case NEVER:
            return 'N';
        default:
            return '?';
    }
}

void render_taskInfo(struct PROC_Response_Task_Info *tinfo, int scroll_step, struct WindowContext *context, struct Location *textBox, int *hovered_tid){
    char infoBuf[20];
    char mouse_over = 0;
    if(context->localMouseX >= 0 && context->localMouseX < 8 && context->localMouseY > (textBox->y + tinfo->task_id*8) && context->localMouseY < ((textBox->y + tinfo->task_id*8) + 8)){
        mouse_over = 1;
        *hovered_tid = tinfo->task_id;
    }
    int info_len = snprintf(infoBuf, sizeof(infoBuf), "%c%d%c", mouse_over ? 'X' : 'O', tinfo->pid, sched_to_char(tinfo->schedule_type));
    int len = strlen(tinfo->name);
    int lscroll = scroll_step % len;
    for(int c = 0; c < info_len; c++){
        vp_drawChar(
            context->viewport, 
            c*8,
            textBox->y+tinfo->task_id*8, 
            infoBuf[c], 
            0x0, mouse_over ? 0xFFFF00 : 0xFFFFFF
        );
    }
    for(int c = lscroll; c < len && c-lscroll < 20; c++){
        vp_drawChar(
            context->viewport, 
            textBox->x+(c-lscroll)*8, 
            textBox->y+tinfo->task_id*8, 
            tinfo->name[c], 
            0xFFFFFF, 0x0
        );
    }
}

int main(int argc, char **argv){
    FILE *task_file = fopen("/-/proc/task", "rw");
    struct Context taskContext;
    
    gui_setup();
    struct WindowContext *context = gui_makeContext("Task MGR", 25*8, 30*8, 10, 0x0);
    struct GUIBar *mainBar = gui_makeBar(context->width, 12, 0xc9c9c9, 0xb9b9b9, 4);
    gui_setLocation(mainBar, 0, 0);
    gui_addChild(context, mainBar);

    struct GUIButton *reloadBtn = gui_makeButton("Reload", ColorText, ColorBG);
    struct GUIButton *exitBtn = gui_makeButton("Exit", ColorText, ColorBG);
    gui_setLocation(reloadBtn, 8, 2);
    gui_setScale(reloadBtn, strlen(reloadBtn->text)*8, 8);
    gui_buttonSetHover(reloadBtn, ColorBG, ColorText);

    gui_setLocation(exitBtn, 2*8+reloadBtn->location.w, 2);
    gui_setScale(exitBtn, strlen(exitBtn->text)*8, 8);
    gui_buttonSetHover(exitBtn, ColorBG, ColorText);

    gui_addChild(mainBar, reloadBtn);
    gui_addChild(mainBar, exitBtn);

    //struct GUIScroll *mainScroll = gui_makeVScroll(12, 59*8-mainBar->location.h+7, 30, 0xc9c9c9, 0xb9b9b9, 0x0F0F0F);
    //gui_setLocation(mainScroll, 0, mainBar->location.h);
    //gui_addChild(context, mainScroll);

    struct Location textBox = {5*8, 13, context->viewport->loc.w - 24, context->viewport->loc.h - 24};

    updateTaskContext(task_file, &taskContext);
    float scroll_ministep = 0.0f;
    int scroll_step = 0;
    int reload_step = 0;

    char infoBuf[20];
    int hovered_tid;
    char click_lock = 0;
    running = 1;
    while(running){
        hovered_tid = -1;
        gui_handleContext(context);
        if(reloadBtn->isClicked){
            updateTaskContext(task_file, &taskContext);
            reloadBtn->isClicked = 0;
        }
        if(exitBtn->isClicked){
            running = 0;
            exitBtn->isClicked = 0;
        }

        for(int i = 0; i < taskContext.ntasks && i < 30; i++){
            struct PROC_Response_Task_Info *tinfo = &taskContext.taskInfo[i];
            render_taskInfo(tinfo, scroll_step, context, &textBox, &hovered_tid);
        }
        scroll_ministep += 0.02f;
        if(scroll_ministep > 1.0f){
            scroll_ministep = 0.0f;
            scroll_step++;
            reload_step++;
        }
        if(reload_step == 4){
            updateTaskContext(task_file, &taskContext);
            reload_step = 0;
        }
        if(context->mouseStatus.buttons.left){
            if(!click_lock){
                click_lock = 1;
                if(hovered_tid != -1){
                    if(taskContext.taskInfo[hovered_tid].schedule_type == NEVER){
                        resume_task(task_file, hovered_tid);
                    }
                    else{
                        pause_task(task_file, hovered_tid);
                    }
                }
            }
        }
        else{
            click_lock = 0;
        }

        vp_copy(context->viewport);
        yield();
    }

    gui_closeContext(context);
    fclose(task_file);
    return 0;
}
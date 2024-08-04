#include "mouse.h"
#include "multitasking.h"

struct MouseStatus mouseStatus;

void mouse_init(){
    mouseStatus.pos.x = fb_width / 2;
    mouseStatus.pos.y = fb_height / 2;
    mouseStatus.buttons.right = 0;
    mouseStatus.buttons.left = 0;
    mouseStatus.buttons.middle = 0;
}

void mouse_callEventHandler(){
	for(int i = 0; i < MAX_TASKS; i++){
		if(
			tasks[i].slot_active && 
			tasks[i].program_slot != -1 && 
			tasks[i].mouse_event_handler != NULL && 
			tasks[i].window == &windows[window_selected]
		){
            select_program(tasks[i].program_slot);
			tasks[i].mouse_event_handler();
			if(tasks[task_running_idx].program_slot != -1){
				select_program(tasks[task_running_idx].program_slot);
			}
		}
	}
}

void mouse_update(int deltaX, int deltaY, struct MouseButtons btns){
    mouseStatus.pos.x += deltaX;
    mouseStatus.pos.y -= deltaY;
    mouseStatus.buttons = btns;
    if(mouseStatus.pos.x < 0) mouseStatus.pos.x = 0;
    else if(mouseStatus.pos.x > (int) fb_width) mouseStatus.pos.x = fb_width - 1;

    if(mouseStatus.pos.y < 0) mouseStatus.pos.y = 0;
    else if(mouseStatus.pos.y > (int) fb_height) mouseStatus.pos.y = fb_height - 1;

    mouseStatus.lastDelta.x = deltaX;
    mouseStatus.lastDelta.y = deltaY;
    mouse_callEventHandler();
}

struct IVec2 mouse_getPos(){
    return mouseStatus.pos;
}
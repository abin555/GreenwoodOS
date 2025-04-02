#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/task.h>

int running;

void explorer_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
	if(event == VP_MAXIMIZE/* || event == VP_FOCUSED */){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE/* || event == VP_UNFOCUSED */){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
    }
    else if(event == VP_FOCUSED){
        set_schedule(ALWAYS);
    }
    else if(event == VP_UNFOCUSED){
        set_schedule(NEVER);
    }
}

int main(int argc, char **argv){
    struct Viewport *vp = vp_open(200, 300, "Calculator");
    vp_add_event_handler(vp, explorer_event);
    running = 1;

    while(running){
        
    }
    vp_close(vp);
}


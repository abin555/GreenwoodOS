#include <stdio.h>
#include <sys/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/task.h>

typedef enum {
	KERNEL = 1,
	DRIVER = 2,
	ALLOC = 3,
	SYSTEM = 4,
	OTHER = 5,
	PROGRAM = 6,
	UNDEFINED = 7,
	FRAMEBUFFER = 8,
	AVAILABLE = 9,
	STACK = 10
} MEMORY_REGION_TYPE;

struct memory_region{
	uint8_t exists;
	uint8_t available;
	MEMORY_REGION_TYPE type;
	uint32_t physical_addr;
	uint32_t virtual_addr;
};

void load_state(int fd, int size, struct memory_region *region_state){
    lseek(fd, 0, SEEK_SET);
    read(fd, region_state, size);
}

int running;

void gui_event(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
	if(event == VP_MAXIMIZE/* || event == VP_FOCUSED */){
        set_schedule(ALWAYS);
    }
    else if(event == VP_MINIMIZE/* || event == VP_UNFOCUSED */){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
        set_schedule(ALWAYS);
    }
}

uint32_t type_to_color(MEMORY_REGION_TYPE type){
    switch(type){
        case KERNEL:
            return 0xFF0000;//RED
            break;
        case DRIVER:
            return 0xFFFF00;//YELLOW
            break;
        case ALLOC:
            return 0x00FFFF;//CYAN
            break;
        case SYSTEM:
            return 0xFF00FF;//Magenta
            break;
        case OTHER:
            return 0x00FF00;//Green
            break;
        case PROGRAM:
            return 0x0000FF;//Blue
            break;
        case UNDEFINED:
            return 0x707070;//Grey
            break;
        case FRAMEBUFFER:
            return 0xff7b00;//Orange
            break;
        case AVAILABLE:
            return 0x000000;//Black
            break;
        case STACK:
            return 0xff7b00;
            break;
    }
}

char type_to_char(MEMORY_REGION_TYPE type){
    switch(type){
        case KERNEL:
            return 'K';
            break;
        case DRIVER:
            return 'D';
            break;
        case ALLOC:
            return 'A';
            break;
        case SYSTEM:
            return 'S';
            break;
        case OTHER:
            return 'O';
            break;
        case PROGRAM:
            return 'P';
            break;
        case UNDEFINED:
            return 'U';
            break;
        case FRAMEBUFFER:
            return 'F';
            break;
        case AVAILABLE:
            return 'A';
            break;
        case STACK:
            return 'T';
            break;
    }
}

int main(void){
    set_schedule(ALWAYS);
    int mmap_fd = open("/-/sys/mmap", O_READ);
    if(mmap_fd == -1) return 1;
    int size = lseek(mmap_fd, 0, SEEK_END);
    int n_entries = size / sizeof(struct memory_region);
    printf("Mmap is %d bytes, with %d entries\n", size, n_entries);

    struct memory_region *region_state = malloc(sizeof(struct memory_region) * n_entries);
    
    load_state(mmap_fd, size, region_state);

    struct Viewport *vp = vp_open(8*32, 8*32, "Memory Map");
    uint32_t *buf = malloc(sizeof(uint32_t) * vp->loc.w * vp->loc.h);
    vp_set_buffer(vp, buf, sizeof(uint32_t) * vp->loc.w * vp->loc.h);
    vp_add_event_handler(vp, gui_event);
    running = 1;
    while(running){
        load_state(mmap_fd, size, region_state);

        int idx = 0;
        for(int y = 0; y < 32; y++){
            for(int x = 0; x < 32; x++){
                if(region_state[idx].available){
                    vp_drawChar(vp, x*8, y*8, 'A', 0xFFFFFF, 0x0);
                }
                else{
                    uint32_t color = type_to_color(region_state[idx].type);
                    char c = type_to_char(region_state[idx].type);
                    vp_drawChar(vp, x*8, y*8, c, 0xFFFFFF, color);
                }                
                idx++;
            }
        }
        vp_copy(vp);
        yield();
    }
    close(mmap_fd);

    return 0;
}
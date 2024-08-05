#include "libc.h"

typedef unsigned char uint8_t;
typedef char int8_t;

struct task_registers{
    uint32_t esp;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;

    uint32_t edx;
    uint32_t ecx;
    uint32_t ebx;
    uint32_t eax;
    uint32_t eip;
};

struct DIRECTORY{
	char path[100];
};

struct task_state{
    struct task_registers registers;
    int8_t program_slot;//0-9 Represents a slot with a need to switch the program slot context; | value of -1 means this is a kernel task with no page change needed.
    char *task_name;
    uint8_t slot_active;
    uint8_t slot_running;
    uint32_t stack_region;
    ScheduleType schedule_type;
    struct WINDOW *window;
    struct CONSOLE *console;
    struct DIRECTORY currentDirectory;
    void (*keyboard_event_handler)(unsigned int);
    void (*mouse_event_handler)(void);
    void (*end_callback)(void);
    bool own_window;
    bool own_console;
};

#define BYTES_PER_LINE 16
#define marginX 5
#define marginY 2

int scan;
struct WINDOW *window;
uint32_t *win_buf;

void memset(void *mem, char v, int size);
void render_memory(char *buf, int line);

int main(int argc, char **argv){
    print("Listing All Active Tasks:\n");
    struct FEATURE_INFO task_feature = getKernelFeature(FEAT_TASKTABLE);
    struct task_state *tasks = task_feature.addr;
    struct FEATURE_INFO pgrm_feature = getKernelFeature(FEAT_PROGRAMBASE);
    uint32_t *program_base = pgrm_feature.addr;
    for(int i = 0; i < task_feature.size / sizeof(struct task_state); i++){
        if(!tasks[i].slot_active) continue;
        print_arg("%d - ", i);
        print_arg("%s", (uint32_t) tasks[i].task_name);
        if(tasks[i].program_slot != -1){
            print_arg(" PGRM #%d", tasks[i].program_slot);
            print_arg(" @0x%x", (uint32_t) (((uint32_t) program_base) + pgrm_feature.size*tasks[i].program_slot));
        }
        print("\n");
    }

    print("Please input task # to manage!\n");
    window_update();

    char task_c = getc();

    if(!(task_c == '0' || (task_c <= '9' && task_c >= '1'))){
        print_arg("Invalid Index %c\n", task_c);
        return 1;
    }
    int task_id;
    if(task_c == '0') task_id = 0;
    else task_id = task_c - '1' + 1;
    if(!tasks[task_id].slot_active){
        print("Task is not available!\n");
        return 1;
    }

    print_arg("Managing Task %s\n", (uint32_t) tasks[task_id].task_name);
    char *memory;
    if(tasks[task_id].program_slot != -1)
        memory = (char *) (((uint32_t) program_base) + pgrm_feature.size*tasks[task_id].program_slot);
    else memory = (uint32_t *) 0xC0000000;
    scan = 0;
    window = window_open("TASKMGR", 1);
    set_schedule(ONFOCUS);
    win_buf = window->backbuffer;
    window_update();
    int line = 0;
    while(1){
        render_memory(memory, line);
        window_update();
        char c = getc();

        if(c == 0x13 && line > 0){
			line--;
            if(scan >= 0){
                scan -= 2*BYTES_PER_LINE;
            }
		}
		else if(c == 0x14){
			line++;
            scan += 2*BYTES_PER_LINE;
		}
        else if(c == 's'){
            line+=10;
            scan += 2*10*BYTES_PER_LINE;
        }
        else if(c == 'w'){
            line-=10;
            scan-=2*10*BYTES_PER_LINE;
        }
		else if(c == 0x11) scan--;
		else if(c == 0x12) scan++;
        else if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')){
            if(c >= '0' && c <= '9') c = c - '0';
            else if(c >= 'a' && c <= 'f')  c = c - 'a' + 0xA;
            else if(c >= 'A' && c <= 'F') c = c - 'A' + 0xA;
            int idx = scan / 2;
            int offset = scan % 2;//if non zero, editing second half of byte.

            uint8_t byte = memory[idx];
            if(offset){
                byte = (byte & 0xF0) | (c & 0xF);
            }
            else{
                byte = (byte & 0x0F) | ((c & 0xF) << 4);
                //byte = byte >> 8 | ((c & 0xF) << 8);
                //byte = 0x12;
            }
            memory[idx] = byte;
            scan++;
        }
        if(scan > 0x400000 * 2) scan = 0x400000 * 2;
        if(scan < 0) scan = 0;
        
    }


    return 0;
}


typedef unsigned char uint8_t;

char quadToChar(uint8_t quad){
    switch(quad & 0x0F){
        case 0x0:
            return '0';
        case 0x1:
            return '1';
        case 0x2:
            return '2';
        case 0x3:
            return '3';
        case 0x4:
            return '4';
        case 0x5:
            return '5';
        case 0x6:
            return '6';
        case 0x7:
            return '7';
        case 0x8:
            return '8';
        case 0x9:
            return '9';
        case 0xA:
            return 'A';
        case 0xB:
            return 'B';
        case 0xC:
            return 'C';
        case 0xD:
            return 'D';
        case 0xE:
            return 'E';
        case 0xF:
            return 'F';
    }
}

void print_byte(uint8_t byte, int x, int y){
    char part1 = quadToChar(byte >> 4);
    char part2 = quadToChar(byte & 0xF);

    drawChar(x*8, y*8, part1);
    drawChar((x+1)*8, y*8, part2);
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}

void render_memory(char *buf, int line){
    int idx = line * BYTES_PER_LINE;
    int x = marginX;
    int y = marginY;

    memset(win_buf, 0, window->width*window->height*sizeof(uint32_t));
    for(int margin = 0; margin < BYTES_PER_LINE; margin++){
        print_byte(margin, (margin*3) + marginX, 0);
    }
    for(int margin = 0; margin < (window->height / 8) - 2 * marginY; margin++){
        print_byte((idx+(margin<<4)) >> 8, 0, margin + marginY);
        print_byte((idx+(margin<<4)) & 0x00FF, 2, margin + marginY);
    }
    for(; idx < 0x400000 && y < (window->height / 8) - marginY; idx++){
        print_byte(buf[idx], x, y);        

        if(idx == scan / 2){
            int offset = 0;
            if(scan % 2){
                offset = 8;
            }
            for(int i = 0; i < 8; i++){
                win_buf[x*8+i+offset + (y*8+7)*window->width] = 0xFF0000;
                win_buf[x*8+i+offset + (y*8-1)*window->width] = 0xFF0000;
            }
        }

        x+=3;
        drawChar((marginX + 3*BYTES_PER_LINE + 1 + ((x-marginX)/3))*8, y*8, buf[idx]);
        if(x == BYTES_PER_LINE * 3 + marginX){
            x = marginX;
            y++;
        }
    }
}
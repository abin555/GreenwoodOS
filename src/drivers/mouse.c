#include "mouse.h"

#define Reset 0xFF
#define Resend 0xFE
#define Error 0xFC
#define Ack 0xFA
#define SelfTestPass 0xAA
#define SetDefault 0xF6
#define Disable 0xF5
#define Enable 0xF4
#define SetSampleRate 0xF3
#define ReadDevType 0xF2
#define SetRemoteMode 0xF0
#define SetWrapMode 0xEE
#define ResetWrapMode 0xEC
#define ReadData 0xEB
#define SetStreamMode 0xEA
#define StatusRequest 0xE9
#define SetResolution 0xE8
#define SetScaling2_1 0xE7
#define SetScaling1_1 0xE6

#define MOUSE_CMD 0x64
#define MOUSE_DATA 0x60
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

uint8_t mouse_step = 0;
uint8_t mouse_byte[3];
int mouse_x;
int mouse_y;
int move_x;
int move_y;
bool mouse_left_click;
bool mouse_right_click;
bool mouse_middle_click;

void mouse_wait(uint8_t a_type) {
	uint32_t timeout = 100000;
	if (!a_type) {
		while (--timeout) {
			if ((inb(MOUSE_CMD) & MOUSE_BBIT) == 1) {
				return;
			}
		}
		printk("[MOUSE] Timeout!\n");
		return;
	} else {
		while (--timeout) {
			if (!((inb(MOUSE_CMD) & MOUSE_ABIT))) {
				return;
			}
		}
		printk("[MOUSE] Timeout!\n");
		return;
	}
}


void mouse_send(uint8_t byte){
    mouse_wait(1);
    outb(MOUSE_CMD, 0xD4);
    mouse_wait(1);
    outb(MOUSE_DATA, byte);
}

uint8_t mouse_read(){
    mouse_wait(0);
    uint8_t t = inb(MOUSE_DATA);
    return t;
}

void mouse_calculate_position(){
    mouse_step = 0;
    uint8_t state = mouse_byte[0];
    move_x = mouse_byte[1];
    move_y = mouse_byte[2];

    if(move_x && state & (1 << 4)){
        move_x = move_x - 0x100;
    }
    if(move_y && state & (1 << 5)){
        move_y = move_y - 0x100;
    }
    if(state & (1 << 6) || state & (1 << 7)){
        //Overflow
        move_x = 0;
        move_y = 0;
    }

    mouse_x += move_x;
    mouse_y -= move_y;

    mouse_left_click = false;
    mouse_middle_click = false;
    mouse_right_click = false;

    if(state & 0b1){
        mouse_left_click = true;
    }
    if(state & 0b10){
        mouse_right_click = true;
    }
    if(state & 0b100){
        mouse_middle_click = true;
    }
    if(mouse_x < 0){
        mouse_x = 0;
    }
    if(mouse_x > (int) fb_width-12){
        mouse_x = fb_width-12;
    }
    if(mouse_y < 0){
        mouse_y = 0;
    }
    if(mouse_y > (int) fb_height-12){
        mouse_y = fb_height-12;
    }
}

struct cpu_state mouse_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    uint8_t status = inb(MOUSE_CMD);
    if((status & MOUSE_BBIT) && (status & MOUSE_F_BIT)){
        uint8_t mouse_in = inb(MOUSE_DATA);
        pic_acknowledge(44);
        switch(mouse_step){
            case 0:
                mouse_byte[0] = mouse_in;
                if(!(mouse_in & MOUSE_V_BIT)) break;
                mouse_step++;
                break;
            case 1:
                mouse_byte[1] = mouse_in;
                mouse_step++;
                break;
            case 2:
                mouse_byte[2] = mouse_in;
                
                mouse_calculate_position();

                break;        
        }
    }
    else{
        pic_acknowledge(44);
    }
    return cpu;
}

#define mouse_ack(); if(mouse_read() != 0xFA) {printk("[MOUSE] No Ack\n");return;}

void initialize_ps2_mouse(){
    printk("[MOUSE] Initializing\n");
    IRQ_OFF;
    uint8_t status;
    uint8_t mouse_id;
    for(int reset_count = 0; reset_count < 4; reset_count++){
        mouse_send(Reset);
        if(mouse_read() != Ack){
            printk("[MOUSE] No Ack on %1x\n", reset_count);
            return;
        }
        uint8_t testSts = mouse_read();
        if(testSts != SelfTestPass && testSts != Ack){
            printk("[MOUSE] Error on %1x %2x\n", reset_count, testSts);
            return;
        }
        mouse_id = mouse_read();
    }
    printk("[MOUSE] ID: %2x\n", mouse_id);
    
    IRQ_clear_mask(2);
    IRQ_clear_mask(12);
    interrupt_add_handle(44, &mouse_handler);

    mouse_send(SetSampleRate);
    mouse_ack();
    mouse_send(10);
    mouse_ack();
    
    mouse_send(ReadDevType);
    mouse_ack();
    mouse_id = mouse_read();
    printk("[MOUSE] ID: %2x\n", mouse_id);

    mouse_send(SetResolution);
    mouse_ack();
    mouse_send(0x03);
    mouse_ack();
    
    mouse_send(SetScaling1_1);
    mouse_ack();

    mouse_send(SetSampleRate);
    mouse_ack();
    mouse_send(80);
    mouse_ack();

    outb(MOUSE_CMD, 0x20);
    mouse_wait(0);
    status = inb(0x60);
    printk("[Mouse] Status: %2x\n", status);
    status |= 2;
    mouse_wait(1);
    outb(MOUSE_CMD, 0x60);
    mouse_wait(1);
    outb(MOUSE_DATA, status);

    outb(MOUSE_CMD, 0x20);
    mouse_wait(0);
    status = inb(0x60);
    printk("[Mouse] Status: %2x\n", status);

    mouse_send(Enable);
    mouse_ack();

    mouse_x = fb_width/2;
    mouse_y = fb_height/2;
    mouse_left_click = false;
    mouse_right_click = false;
    mouse_middle_click = false;
    IRQ_RES;
}
FILE* mouse_cursor;
uint32_t *mouse_cursor_data;
bool render_mouse = false;
bool mouse_enable;
void init_mouse(){
    printk("[MOUSE] Rendering Init\n");
    mouse_cursor = fopen("./OS/CURSOR.BIN");
    if(mouse_cursor->size != 0){
        render_mouse = true;
        mouse_cursor_data = (uint32_t *) malloc(mouse_cursor->size);
        uint32_t *file_sector_data = (uint32_t *) fread(mouse_cursor->drive, mouse_cursor->sector);
        memcpy(file_sector_data, mouse_cursor_data, mouse_cursor->size);
    }
    else{
        render_mouse = false;
        printk("[MOUSE] Cursor not found!\n");
    }
    fclose(mouse_cursor);
    timer_attach(10, mouse_timer_routine);
    mouse_enable = true;
}

void mouse_timer_routine(){
    if(!mouse_enable) return;
    mouse_draw();
}

void mouse_draw(){
    if(!(mouse_x >= 0 && mouse_y >= 0 && mouse_x < (int) fb_width-12 && mouse_y < (int) fb_height-21)){
        return;
    }
    if(render_mouse){
        for(int y = 0; y < 21; y++){
            for(int x = 0; x < 12; x++){
                uint32_t pixel = mouse_cursor_data[x + y*12+1];
                if(pixel){
                    framebuffer[
                        (mouse_x + x) + ((mouse_y + y) * fb_width)
                    ] = pixel;
                }
            }
        }
    }
    else{
        fb_setPixel(
            (fb_width/2) + mouse_x,
            (fb_height/2) + mouse_y,
            0x5050FF
        );
    }
}
#include "mouse.h"

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define PS2_ACK 0xFA

uint8_t mouse_device;
int32_t mouse_x,mouse_y;
char mouse_left_pressed;
char mouse_right_pressed;
char mouse_middle_pressed;
uint32_t current_byte = 0;
uint32_t bytes_per_packet = 3;
uint8_t packet[4] = { 0 };
uint32_t mouse_replace_pixels[8*8];

char mouse_img[8] = {
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111110,
    0b11111111
};


void mouse_interrupt_handler(){

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            fb_setPixel(mouse_x+x, mouse_y+y, mouse_replace_pixels[y*8+x]);
        }
    }

    //fb_setPixel(x, y, 0);
    uint8_t byte = ps2_read(PS2_DATA);
    if(current_byte == 0 && !(byte & MOUSE_ALWAYS_SET)){
        return;
    }
    packet[current_byte] = byte;
    current_byte = (current_byte + 1) % bytes_per_packet;
    if(current_byte == 0){
        mouse_handle_packet();
    }
    //fb_setPixel(x, y, 0xFF00FF);

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            mouse_replace_pixels[y*8+x] = fb[((mouse_y + y)) * fb_width + (mouse_x + x)];
            //fb_setPixel(mouse_x+x, mouse_y+y, ((mouse_img[y] << x) & 0b10000000) ? 0xFFFFFF : mouse_replace_pixels[y*8+x]);
            fb_setPixel(mouse_x+x, mouse_y+y, 0xFFFFFF);
        }
    }
}

void mouse_handle_packet(){
    uint8_t flags = packet[0];
    int32_t delta_x = (int32_t) packet[1];
    int32_t delta_y = (int32_t) packet[2];
    uint8_t extra = 0;

    if(bytes_per_packet == 4){
        extra = packet[3];

        if(extra & MOUSE_UNUSED_A || extra & MOUSE_UNUSED_B){
            return;
        }
    }

    if(flags & MOUSE_X_OVERFLOW || flags & MOUSE_Y_OVERFLOW){
        return;
    }
    if(flags & MOUSE_X_NEG){
        delta_x |= 0xFFFFFF00;
    }

    if(flags & MOUSE_Y_NEG){
        delta_y |= 0xFFFFFF00;
    }

    mouse_left_pressed = flags & MOUSE_LEFT;
    mouse_right_pressed = flags & MOUSE_RIGHT;
    mouse_middle_pressed = flags & MOUSE_MIDDLE;

    //mouse_x += delta_x;
    //mouse_y -= delta_y;

    if((mouse_x + delta_x) > 0 && (mouse_x + delta_x) <= (int) fb_width){
        mouse_x += delta_x;
    }
    else{
        //mouse_x += -delta_x;
    }
    if((mouse_y - delta_y) > 0 && (mouse_y - delta_y) <= (int) fb_height){
        mouse_y -= delta_y;
    }
    else{
        //mouse_y -= -delta_y;
    }

    if(mouse_left_pressed){
        printk("Mouse Left\n");
    }

    //printk("Mouse X: %h Mouse Y: %h\n", x, y);
}


void mouse_init(uint8_t device){
    printk("[ps2] Initializing Mouse\n");
    IRQ_OFF;
    mouse_device = device;
    mouse_x = fb_width / 2;
    mouse_y = fb_height / 2;
    mouse_left_pressed = 0;
    mouse_right_pressed = 0;
    mouse_middle_pressed = 0;

    // Enable features
    mouse_enable_scroll_wheel();
    mouse_enable_five_buttons();

    // Set mouse parameters
    mouse_set_sample_rate(80);
    mouse_set_resolution(0x00); // One unit per mm
    mouse_set_scaling(false);   // Disable acceleration

    ps2_write_device(device, PS2_DEV_ENABLE_SCAN);
    ps2_expect_ack();

    interrupt_add_handle(44, &mouse_interrupt_handler);
    IRQ_RES;
}

void mouse_set_sample_rate(uint8_t rate){
    ps2_write_device(mouse_device, MOUSE_SET_SAMPLE);
    ps2_expect_ack();
    ps2_write_device(mouse_device, rate);
    ps2_expect_ack();
}

void mouse_set_resolution(uint8_t level){
    ps2_write_device(mouse_device, MOUSE_SET_RESOLUTION);
    ps2_expect_ack();
    ps2_write_device(mouse_device, level);
    ps2_expect_ack();
}

void mouse_set_scaling(char enabled) {
    uint8_t cmd = enabled ? MOUSE_ENABLE_SCALING : MOUSE_DISABLE_SCALING;

    ps2_write_device(mouse_device, cmd);
    ps2_expect_ack();
}

/* Uses a magic sequence to enable scroll wheel support.
 */
void mouse_enable_scroll_wheel() {
    mouse_set_sample_rate(200);
    mouse_set_sample_rate(100);
    mouse_set_sample_rate(80);

    uint32_t type = ps2_identify_dev(mouse_device);

    if (type == PS2_MOUSE_SCROLL_WHEEL) {
        bytes_per_packet = 4;
        printk("[MOUSE] Enabled scroll wheel\n");
    } else {
        printk("[MOUSE] Unable to enable scroll wheel\n");
    }
}

/* Uses a magic sequence to enable five buttons support.
 */
void mouse_enable_five_buttons() {
    if (bytes_per_packet != 4) {
        return;
    }

    mouse_set_sample_rate(200);
    mouse_set_sample_rate(200);
    mouse_set_sample_rate(80);

    uint32_t type = ps2_identify_dev(mouse_device);

    if (type != PS2_MOUSE_FIVE_BUTTONS) {
        printk("[MOUSE] Mouse has fewer than five buttons\n");
    } else {
        printk("[MOUSE] Five buttons enabled\n");
    }
}
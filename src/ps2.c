#include "ps2.h"
#include "framebuffer.h"

char ps2_controllers[] = {1, 1};

void ps2_init(){
    print_serial("[PS2] Init PS/2 Devices\n");
    char dual_channel = 1;

    IRQ_OFF;
    
    //PS2 Controller Self Test

    ps2_write(PS2_CMD, PS2_DISABLE_FIRST);
    ps2_write(PS2_CMD, PS2_DISABLE_SECOND);

    inb(PS2_DATA);

    ps2_write(PS2_CMD, PS2_READ_CONFIG);    

    uint8_t config = ps2_read(PS2_DATA);
    //uint8_t original_config = config;

    /*
    for(int i = 0; i < 8; i++){
        fb_putChar(8*i, 8*6, (config >> (7 - i)) & 1 ? '1' : '0', 0xFFFFFF, 0x0);
    }
    */

    config |= PS2_CFG_SYSTEM_FLAG;

    if(config & PS2_CFG_MUST_BE_ZERO){
        print_serial("[PS2] Invalid bit set in config\n");
    }

    config &= ~(PS2_CFG_FIRST_PORT | PS2_CFG_SECOND_PORT | PS2_CFG_TRANSLATION);

    /*
    for(int i = 0; i < 8; i++){
        fb_putChar(8*i, 8*7, (config >> (7 - i)) & 1 ? '1' : '0', 0xFFFFFF, 0x0);
    }
    */

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    inb(PS2_DATA);

    for(int j = 0; j < 5; j++){
        fb_putChar(8, 8*j, 'T', 0xFF0000, 0x0000FF);
        ps2_write(PS2_CMD, PS2_SELF_TEST);
        if(ps2_read(PS2_DATA) != PS2_SELF_TEST_OK){
            print_serial("[PS2] Controller failed self-test\n");
            ps2_controllers[0] = 1;
            ps2_controllers[1] = 1;
            //fb_putChar(0, 8*j, 'X', 0xFF0000, 0x0000FF);
            //for(int i = 0; i < 0xFFFFFFF; i++){}
            //return;
        }
        else{
            ps2_controllers[0] = 1;
            ps2_controllers[1] = 1;
            //fb_putChar(8, 8*j, 'T', 0x00FF00, 0x000000);
            //for(int i = 0; i < 0xFFFFFFF; i++){}
            break;
        }
    }

    

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
    ps2_write(PS2_CMD, PS2_READ_CONFIG);
    config = ps2_read(PS2_DATA);

    /*
    for(int i = 0; i < 8; i++){
        fb_putChar(8*i, 8*8, (config >> (7 - i)) & 1 ? '1' : '0', 0xFFFFFF, 0x0);
    }
    */

    inb(PS2_DATA);
    
    //Controllers Self Test

    if(config & PS2_CFG_SECOND_CLOCK){
        //print_serial("[PS2] Only one PS/2 controller\n");

        //fb_putChar(8*4, 0, '2', 0xFF0000, 0x000000);
        //for(int i = 0; i < 0xFFFFFFF; i++){}
        dual_channel = 0;
        ps2_controllers[1] = 0;
    }
    else{
        fb_putChar(8*4, 0, '2', 0x00FF00, 0x000000);
        //for(int i = 0; i < 0xFFFFFFF; i++){}
        //ps2_write(PS2_CMD, PS2_DISABLE_SECOND);
    }

    inb(PS2_DATA);

    ps2_write(PS2_CMD, PS2_TEST_FIRST);

    if(ps2_read(PS2_DATA) != PS2_TEST_OK){
        //print_serial("[PS2] PS/2 Port 1 failed test\n");
        ps2_controllers[0] = 0;
        //fb_putChar(8*4, 8, '1', 0xFF0000, 0x000000);
        //for(int i = 0; i < 0xFFFFFFF; i++){}
    }
    else{
        //fb_putChar(8*4, 8, '1', 0x00FF00, 0x000000);
        //for(int i = 0; i < 0xFFFFFFF; i++){}
    }

    inb(PS2_DATA);

    if(dual_channel){
        ps2_write(PS2_CMD, PS2_TEST_SECOND);

        if(ps2_read(PS2_DATA) != PS2_TEST_OK){
            //print_serial("[PS2] PS/2 Port 2 failed test\n");
            ps2_controllers[1] = 0;
        }
    }    

    if(ps2_identify_multiplex()){
        ps2_deactivate_multiplexing();
    }

    if(ps2_controllers[0]){
        ps2_write(PS2_CMD, PS2_ENABLE_FIRST);
        config |= PS2_CFG_FIRST_PORT;
        config &= ~PS2_CFG_FIRST_CLOCK;
        print_serial("[PS2] Port 1 Enabled\n");
    }

    if(ps2_controllers[1]){
        ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
        config |= PS2_CFG_SECOND_PORT;
        config &= ~PS2_CFG_SECOND_CLOCK;
        print_serial("[PS2] Port 2 Enabled\n");
    }

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    /*
    for(int i = 0; i < 8; i++){
        fb_putChar(8*i, 8*9, (config >> (7 - i)) & 1 ? '1' : '0', 0xFFFFFF, 0x0);
    }
    */

    
    /*
    for(uint32_t i = 0; i < 2; i++){
        if(!ps2_controllers[i]) continue;
        ps2_read(PS2_DATA);
        ps2_read(PS2_DATA);
        ps2_write_device(i, PS2_DEV_RESET);
        int pass = 0;
        for(int j = 0; j < 5; j++){
            try_again:;
            uint8_t ret[2];
            ret[0] = ps2_read(PS2_DATA);
            if(ret[0] == 0x00) goto try_again;
            ret[1] = ps2_read(PS2_DATA);

            fb_putChar(8*(10 + 3*i), 8*10, quadToHex(ret[0] & 0xF0), 0xFFFFFF, 0);
            fb_putChar(8*(11 + 3*i), 8*10, quadToHex(ret[0] & 0xF), 0xFFFFFF, 0);
            fb_putChar(8*(10 + 3*i), 8*11, quadToHex(ret[1] & 0xF0), 0xFFFFFF, 0);
            fb_putChar(8*(11 + 3*i), 8*11, quadToHex(ret[1] & 0xF), 0xFFFFFF, 0);

            if((ret[0] == PS2_DEV_ACK && ret[1] == PS2_DEV_RESET_ACK) || (ret[0] == PS2_DEV_RESET_ACK && ret[1] == PS2_DEV_ACK)){
                fb_putChar(6*(8*j), 8*i, 'D', 0xFF0000, 0x0);
                pass = 1;
                break;
            }
            else{
                print_serial("[PS2] Failure to reset dev %d\n", i);
                //ps2_controllers[i] = 0;
                //config &= ~(i == 0 ? PS2_CFG_FIRST_PORT : PS2_CFG_SECOND_PORT);
                fb_putChar(6*(8*j), 8*i, 'D', 0xFF0000, 0x0);
            }
        }
        if(!pass){
            ps2_controllers[i] = 0;
            config &= ~(i == 0 ? PS2_CFG_FIRST_PORT : PS2_CFG_SECOND_PORT);
        }
    }

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);
    for(int i = 0; i < 8; i++){
        fb_putChar(8*i, 8*10, (config >> (7 - i)) & 1 ? '1' : '0', 0xFFFFFF, 0x0);
    }
    */

    for(int i = 0; i < 2; i++){
        if(ps2_controllers[i]){
            uint32_t type = ps2_identify_dev(i);
            //fb_putChar(8*i, 8, '0'+type, 0xFFFFFF,0);
            switch(type){
                case PS2_KEYBOARD:
                case PS2_KEYBOARD_TRANSLATED:
                    print_serial("[PS2] Keyboard on %d\n", i);
                    //fb_putChar(i*8, 0, 'K', 0xda42f5, 0x000000);
                    ps2_keyboard_init(i);
                    break;
                case PS2_MOUSE:
                case PS2_MOUSE_SCROLL_WHEEL:
                case PS2_MOUSE_FIVE_BUTTONS:
                    print_serial("[PS2] Mouse on %d\n", i);
                    //fb_putChar(i*8, 0, 'M', 0x42ecf5, 0x000000);
                    ps2_mouse_init(i);
                    break;
            }
        }
    }

    /*
    fb_putChar(16, 0, 'P', 0xed4566, 0x000000);
    for(int i = 0; i < 0xFFFFFFF; i++){}

    for(int i = 0; i < 8; i++){
        fb_putChar(8*i, 8*11, (config >> (7 - i)) & 1 ? '1' : '0', 0xFFFFFF, 0x0);
    }

    for(int i = 0; i < 0xFFFFFFF; i++){}

    for(int i = 0; i < 0xFFFFFFF; i++){}

    for(int i = 0; i < 0xFFFFFFF; i++){}
    */

    IRQ_RES;
    //print_serial("[PS2] Initialized\n");
}

uint32_t ps2_identify_multiplex(){
    ps2_write(PS2_CMD, 0xD3);
    ps2_write(PS2_DATA, 0xF0);
    uint8_t res1 = ps2_read(PS2_DATA);//Should be 0xF0 if multi
    ps2_write(PS2_CMD, 0xD3);
    ps2_write(PS2_DATA, 0x56);
    uint8_t res2 = ps2_read(PS2_DATA);//Should be 0x56 if multi

    fb_putChar(8*(10), 8*4, quadToHex(res1 & 0xF0), 0xFFFFFF, 0);
    fb_putChar(8*(11), 8*4, quadToHex(res1 & 0xF), 0xFFFFFF, 0);
    fb_putChar(8*(10), 8*5, quadToHex(res2 & 0xF0), 0xFFFFFF, 0);
    fb_putChar(8*(11), 8*5, quadToHex(res2 & 0xF), 0xFFFFFF, 0);

    ps2_write(PS2_CMD, 0xD3);
    ps2_write(PS2_DATA, 0xA4);
    uint8_t ver = ps2_read(PS2_DATA);
    fb_putChar(8*(10), 8*6, quadToHex(ver & 0xF0), 0xFFFFFF, 0);
    fb_putChar(8*(11), 8*6, quadToHex(ver & 0xF), 0xFFFFFF, 0);
    print_serial("[PS2] Multiplex Test: %x %x %x\n", res1, res2, ver);
    if(ver != 0xA4){
        print_serial("[PS2] Mutliplex is available!\n");
        return 1;
    }
    return 0;
}

void ps2_deactivate_multiplexing(){
    ps2_write(PS2_CMD, 0xD3);
    ps2_write(PS2_DATA, 0xF0);
    uint8_t res1 = ps2_read(PS2_DATA);//Should be 0xF0 if multi

    ps2_write(PS2_CMD, 0xD3);
    ps2_write(PS2_DATA, 0x56);
    uint8_t res2 = ps2_read(PS2_DATA);//Should be 0xF0 if multi
    
    ps2_write(PS2_CMD, 0xD3);
    ps2_write(PS2_DATA, 0xA5);
    uint8_t ver = ps2_read(PS2_DATA);//Should be 0xF0 if multi

    fb_putChar(8*(14), 8*4, quadToHex(res1 & 0xF0), 0xFFFFFF, 0);
    fb_putChar(8*(15), 8*4, quadToHex(res1 & 0xF), 0xFFFFFF, 0);
    fb_putChar(8*(14), 8*5, quadToHex(res2 & 0xF0), 0xFFFFFF, 0);
    fb_putChar(8*(15), 8*5, quadToHex(res2 & 0xF), 0xFFFFFF, 0);
    fb_putChar(8*(14), 8*6, quadToHex(ver & 0xF0), 0xFFFFFF, 0);
    fb_putChar(8*(15), 8*6, quadToHex(ver & 0xF), 0xFFFFFF, 0);

    print_serial("[PS2] Disabled Multiplexing %x %x %x\n", res1, res2, ver);
}

uint32_t ps2_identify_dev(uint32_t dev_num){
    ps2_write_device(dev_num, PS2_DEV_DISABLE_SCAN);
    ps2_expect_ack();
    ps2_write_device(dev_num, PS2_DEV_IDENTIFY);
    ps2_expect_ack();

    uint32_t first_id_byte = ps2_read(PS2_DATA);
    uint32_t second_id_byte = ps2_read(PS2_DATA);

    return ps2_identify_bytes_to_type(first_id_byte, second_id_byte);
}

uint32_t ps2_identify_bytes_to_type(uint8_t first, uint8_t second){
    if (first == 0x00 || first == 0x03 || first == 0x04) {
        return first; // PS2_MOUSE* match the first byte
    } else if (first == 0xAB) {
        if (second == 0x41 || second == 0xC1) {
            return PS2_KEYBOARD_TRANSLATED;
        } else if (second == 0x83) {
            return PS2_KEYBOARD;
        }
    }

    return PS2_DEVICE_UNKNOWN;
}

char ps2_wait_write(){
    int timer = PS2_TIMEOUT;
    while((inb(0x64) & 2) && timer-- > 0){
        asm("pause");
    }
    return timer != 0;
}

char ps2_wait_read(){
    int timer = PS2_TIMEOUT;
    while(!(inb(0x64) & 1) && timer-- >= 0){
        asm("pause");
    }
    return timer != 0;
}

uint8_t ps2_read(uint32_t port){
    if(ps2_wait_read()){
        return inb(port);
    }
    //print_serial("[PS2] Read failure\n");
    return -1;
}

char ps2_write(uint32_t port, uint8_t b){
    if(ps2_wait_write()){
        outb(port, b);
        return 1;
    }
    //print_serial("[PS2] Write failure\n");
    return 0;
}

char ps2_write_device(uint32_t dev, uint8_t b){
    if(dev != 0){
        if(!ps2_write(PS2_CMD, PS2_WRITE_SECOND)){
            return 0;
        }
    }
    return ps2_write(PS2_DATA, b);
}

char ps2_expect_ack(){
    uint8_t ret = ps2_read(PS2_DATA);
    if(ret != PS2_DEV_ACK){
        //print_serial("[PS2] Device failed to ack cmd\n");
        return 0;
    }
    return 1;
}

uint8_t keyboard_scancode;

struct cpu_state ps2_keyboard_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	pic_acknowledge(33);
	uint8_t scancode = inb(PS2_DATA);
	////print_serial("[PS/2 Keyboard] Interrupt %x\n", scancode);
	kbd_recieveScancode(scancode, PS2_KBD);
	return cpu;
}

void ps2_keyboard_init(int device){
    print_serial("[PS2 KEYBOARD] Initializing\n");

    ps2_write_device(device, PS2_KBD_SSC_CMD);
    ps2_expect_ack();
    ps2_write_device(device, PS2_KBD_SSC_GET);
    ps2_expect_ack();
    keyboard_scancode = ps2_read(PS2_DATA);
    print_serial("[PS2 KEYBOARD] Scancode is currently: %d\n", keyboard_scancode);

    

    print_serial("[PS2 KEYBOARD] Enabling\n");
    ps2_write_device(device, PS2_DEV_ENABLE_SCAN);
    ps2_expect_ack();
    interrupt_add_handle(33, &ps2_keyboard_handler);
    IRQ_clear_mask(1);
    print_serial("[PS2 KEYBOARD] Enabled!\n");
}

uint8_t mouse_cycle;
uint8_t mouse_byte[3];

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

void ps2_mouse_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    //fb_putChar(fb_width - 16, fb_height - 8, 'M', 0xFFFFFF, 0x0);
    uint8_t status = ps2_read(MOUSE_STATUS);
	while (status & MOUSE_BBIT) {
		int8_t mouse_in = ps2_read(MOUSE_PORT);
		if (status & MOUSE_F_BIT) {
			switch (mouse_cycle) {
				case 0:
					mouse_byte[0] = mouse_in;
					if (!(mouse_in & MOUSE_V_BIT)) return;
					++mouse_cycle;
					break;
				case 1:
					mouse_byte[1] = mouse_in;
					++mouse_cycle;
					break;
				case 2:
					mouse_byte[2] = mouse_in;
					/* We now have a full mouse packet ready to use */
					if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
						/* x/y overflow? bad packet! */
						break;
					}
					mouse_cycle = 0;

                    char dX = *((char *) &mouse_byte[1]);
                    char dY = *((char *) &mouse_byte[2]);

                    struct MouseButtons btns;
                    btns.right = (mouse_byte[0] & (1 << 1)) ? 1 : 0;
                    btns.left = (mouse_byte[0] & (1 << 0)) ? 1 : 0;
                    btns.middle = (mouse_byte[0] & (1 << 2)) ? 1 : 0;
                    
                    mouse_update(dX, dY, btns);
			}
		}
		status = inb(MOUSE_STATUS);
	}
    
    
    pic_acknowledge(44);
}

void ps2_mouse_init(int device){
    IRQ_OFF;
    print_serial("[PS2] Initializing Mouse on Dev %d\n", device);
 
    ps2_write_device(device, 0xF6);//Use default settings
    ps2_expect_ack();

    ps2_write_device(device, PS2_DEV_ENABLE_SCAN);
    ps2_expect_ack();

    interrupt_add_handle(44, &ps2_mouse_handler);
    IRQ_clear_mask(12);
    IRQ_clear_mask(2);
    IRQ_RES;
}

#include "ps2.h"
#include "console_old.h"

char ps2_controllers[] = {1, 1};

void ps2_init(){
    printk("[PS2] Init PS/2 Devices\n");
    char dual_channel = 1;

    IRQ_OFF;

    ps2_write(PS2_CMD, PS2_DISABLE_FIRST);
    ps2_write(PS2_CMD, PS2_DISABLE_SECOND);

    inb(PS2_DATA);

    ps2_write(PS2_CMD, PS2_READ_CONFIG);
    

    uint8_t config = ps2_read(PS2_DATA);

    config |= PS2_CFG_SYSTEM_FLAG;

    if(config & PS2_CFG_MUST_BE_ZERO){
        printk("[PS2] Invalid bit set in config\n");
    }

    config &= ~(PS2_CFG_FIRST_PORT | PS2_CFG_SECOND_PORT | PS2_CFG_TRANSLATION);

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    ps2_write(PS2_CMD, PS2_SELF_TEST);
    if(ps2_read(PS2_DATA) != PS2_SELF_TEST_OK){
        printk("[PS2] Controller failed self-test\n");
        ps2_controllers[0] = 0;
        ps2_controllers[1] = 0;
        return;
    }

    

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
    ps2_write(PS2_CMD, PS2_READ_CONFIG);
    config = ps2_read(PS2_DATA);

    

    if(config & PS2_CFG_SECOND_CLOCK){
        printk("[PS2] Only one PS/2 controller\n");
        dual_channel = 0;
        ps2_controllers[1] = 0;
    }
    else{
        ps2_write(PS2_CMD, PS2_DISABLE_SECOND);
    }

    ps2_write(PS2_CMD, PS2_TEST_FIRST);

    if(ps2_read(PS2_DATA) != PS2_TEST_OK){
        printk("[PS2] PS/2 Port 1 failed test\n");
        ps2_controllers[0] = 0;
    }

    if(dual_channel){
        ps2_write(PS2_CMD, PS2_TEST_SECOND);

        if(ps2_read(PS2_DATA) != PS2_TEST_OK){
            printk("[PS2] PS/2 Port 2 failed test\n");
            ps2_controllers[1] = 0;
        }
    }

    

    if(ps2_controllers[0]){
        ps2_write(PS2_CMD, PS2_ENABLE_FIRST);
        config |= PS2_CFG_FIRST_PORT;
        config &= ~PS2_CFG_FIRST_CLOCK;
        printk("[PS2] Port 1 Enabled\n");
    }

    if(ps2_controllers[1]){
        ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
        config |= PS2_CFG_SECOND_PORT;
        config &= ~PS2_CFG_SECOND_CLOCK;
        printk("[PS2] Port 2 Enabled\n");
    }

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    

    for(uint32_t i = 0; i < 2; i++){
        if(!ps2_controllers[i]) continue;

        ps2_write_device(i, PS2_DEV_RESET);
        uint8_t ret = ps2_read(PS2_DATA);

        if(ret != PS2_DEV_ACK || ps2_read(PS2_DATA) != PS2_DEV_RESET_ACK){
            printk("[PS2] Failure to reset dev %1x\n", i);
            ps2_controllers[i] = 0;
            config &= ~(i == 0 ? PS2_CFG_FIRST_PORT : PS2_CFG_SECOND_PORT);

            ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
            ps2_write(PS2_DATA, config);
        }
    }

    for(int i = 0; i < 2; i++){
        if(ps2_controllers[i]){
            uint32_t type = ps2_identify_dev(i);
            switch(type){
                case PS2_KEYBOARD:
                case PS2_KEYBOARD_TRANSLATED:
                    printk("[PS2] Keyboard on %x\n", i);
                    ps2_keyboard_init(i);
                    break;
                case PS2_MOUSE:
                case PS2_MOUSE_SCROLL_WHEEL:
                case PS2_MOUSE_FIVE_BUTTONS:
                    printk("[PS2] Mouse on %x\n", i);
                    break;
            }
        }
    }


    IRQ_RES;
    printk("[PS2] Initialized\n");
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
    printk("[PS2] Read failure\n");
    return -1;
}

char ps2_write(uint32_t port, uint8_t b){
    if(ps2_wait_write()){
        outb(port, b);
        return 1;
    }
    printk("[PS2] Write failure\n");
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
        printk("[PS2] Device failed to ack cmd\n");
        return 0;
    }
    return 1;
}

uint8_t keyboard_scancode;

struct cpu_state ps2_keyboard_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	pic_acknowledge(33);
	uint8_t scancode = inb(PS2_DATA);
	//printk("[PS/2 Keyboard] Interrupt %x\n", scancode);
	kbd_recieveScancode(scancode, PS2_KBD);
	return cpu;
}

void ps2_keyboard_init(int device){
	IRQ_OFF;
    printk("[KEYBOARD] Initializing\n");
    ps2_write_device(device, PS2_KBD_SSC_CMD);
    ps2_expect_ack();
    ps2_write_device(device, PS2_KBD_SSC_2);
    ps2_expect_ack();

    ps2_write_device(device, PS2_KBD_SSC_CMD);
    ps2_expect_ack();
    ps2_write_device(device, PS2_KBD_SSC_GET);
    ps2_expect_ack();
    keyboard_scancode = ps2_read(PS2_DATA);
    printk("[KEYBOARD] Scancode is currently: %d\n", keyboard_scancode);

    if(keyboard_scancode != 2){
        ps2_write_device(device, PS2_KBD_SSC_CMD);
        ps2_expect_ack();
        ps2_write_device(device, 2);
        ps2_expect_ack();

        ps2_write_device(device, PS2_KBD_SSC_CMD);
        ps2_expect_ack();
        ps2_write_device(device, PS2_KBD_SSC_GET);
        ps2_expect_ack();
        keyboard_scancode = ps2_read(PS2_DATA);
        printk("[KEYBOARD] Scancode is currently: %d\n", keyboard_scancode);
    }

    

    printk("[KEYBOARD] Enabling\n");
    ps2_write_device(device, PS2_DEV_ENABLE_SCAN);
    ps2_expect_ack();
    interrupt_add_handle(33, &ps2_keyboard_handler);
    IRQ_clear_mask(1);
    printk("[KEYBOARD] Enabled!\n");
    IRQ_RES;
}

/*
void ps2_mouse_init(int device){

}
*/
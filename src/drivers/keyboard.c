#include "keyboard.h"

uint8_t keyboard_scancode;

uint8_t keystrokes[256];


void initialize_ps2_keyboard(int device){
    IRQ_OFF;
    printk("[KEYBOARD] Initializing\n");
    ps2_write_device(device, KBD_SSC_CMD);
    ps2_expect_ack();
    ps2_write_device(device, KBD_SSC_2);
    ps2_expect_ack();

    ps2_write_device(device, KBD_SSC_CMD);
    ps2_expect_ack();
    ps2_write_device(device, KBD_SSC_GET);
    ps2_expect_ack();

    keyboard_scancode = ps2_read(PS2_DATA);

    printk("[KEYBOARD] Scancode is currently: %2x\n", keyboard_scancode);
    printk("[KEYBOARD] Enabling\n");
    ps2_write_device(device, PS2_DEV_ENABLE_SCAN);
    ps2_expect_ack();
    interrupt_add_handle(33, &keyboard_handler);
    IRQ_clear_mask(2);

    IRQ_RES;
}

void keyboard_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    //pic_acknowledge(33);
    uint8_t scancode = inb(PS2_DATA);
    //printk("[INT] KEYBOARD INTERRUPT | SC: %2x\n", scancode);
    if(scancode){
        if(kbd_US[scancode] != 0){
            if(kbd_flags.shift){
                keyboard_ASCIIBuffer[keyboard_ascii_index] = kbd_US_shift[scancode];
            }
            else{
                keyboard_ASCIIBuffer[keyboard_ascii_index] = kbd_US[scancode];
            }
            keyboard_ascii_index++;
        }
        else{
            switch(scancode){
                case 0x2a:
                kbd_flags.shift = true;
                break;
                case 0xaa:
                kbd_flags.shift = false;
                break;
                case 0x1D:
                kbd_flags.ctrl = true;
                break;
                case 0x9D:
                kbd_flags.ctrl = false;
                break;
                case 0xE0:
                kbd_flags.arrow = true;
                break;
                case 0xF0:
                if(kbd_flags.arrow && scancode & 0x80){
                    kbd_flags.arrow = false;
                }
                break;
            }
        }
        keyboard_KEYBUFFER[keyboard_KEYBUFFER_index] = scancode;
        keyboard_KEYBUFFER_index++;
    }
    if(keyboard_ascii_index >= KEYBOARD_BUFFERSIZE){
        keyboard_ascii_index = 0;
    }
    if(keyboard_KEYBUFFER_index >= KEYBOARD_BUFFERSIZE){
        keyboard_KEYBUFFER_index = 0;
    }
}
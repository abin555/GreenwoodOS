#include "keyboard.h"
#include "console_old.h"

void kbd_init(uint32_t buffer_size){
	keyboard_buffer_size = buffer_size;
	keyboard_KEYBuffer = (uint8_t *) malloc(keyboard_buffer_size);
	keyboard_ASCIIBuffer = (char *) malloc(keyboard_buffer_size);
	KBD_scancode_buffer_idx = 0;
	KBD_ascii_buffer_idx = 0;
	KBD_last_key_idx = 0;
	KBD_flags.shift = 0;
	KBD_flags.ctrl = 0;
	KBD_flags.arrow = 0;
}

void kbd_recieveScancode(uint8_t scancode, KBD_SOURCE source){
	//print_serial("[Keyboard Driver] Keyboard recieved scancode %x from %x\n", scancode, source);
	printk("KEY %x\n", scancode);
	char justRelease = 0;
	char justSpecial = 0;
	if(source != PS2_KBD) return;
	if(scancode){
		if(kbd_US[scancode] != 0 && !KBD_flags.release && !KBD_flags.special){
			if(KBD_flags.ctrl && !KBD_flags.shift){
				//print_serial("Check if Window Switch %c\n", kbd_US[scancode]);
				if(kbd_US[scancode] >= '1' && kbd_US[scancode] <= '9'){
					//print_serial("Is Window Switch\n");
					int new_window = kbd_US[scancode] - '1';
					//print_serial("Window %d\n", new_window);
					if(windows[new_window].active){
						window_selected = new_window;
						window_copy_buffer(&windows[window_selected]);
					}
				}
			}
			else if(KBD_flags.shift){
				keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = kbd_US_shift[scancode];
				KBD_flags.key = kbd_US_shift[scancode];
			}
			else{
				keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = kbd_US[scancode];
				KBD_flags.key = kbd_US[scancode];
			}
			KBD_ascii_buffer_idx++;
			keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
		}
		else{
			switch(scancode){
				case 0xE0:
					KBD_flags.special = true;
					justSpecial = 1;
					break;
				case 0xF0:
					KBD_flags.release = true;
					justRelease = 1;
					break;
				case 0x12:
					if(!KBD_flags.release){
						KBD_flags.shift = true;
					}
					else{
						KBD_flags.shift = false;
					}
					break;
				case 0x14:
					if(!KBD_flags.release){
						KBD_flags.ctrl = true;
					}
					else{
						KBD_flags.ctrl = false;
					}
					break;
				case 0x69:
					if(KBD_flags.special){
						print_serial("End!\n");
						for(int i = 0; i < MAX_TASKS; i++){
							if(tasks[i].window == &windows[window_selected]){
								stop_task(i);
							}
						}
						//reboot();
					}
					break;
			}
			if(KBD_flags.release && !justRelease){
				KBD_flags.release = false;
				KBD_flags.key = 0;
			}
			if(KBD_flags.special && !justSpecial){
				KBD_flags.special = false;
			}
		}
		keyboard_KEYBuffer[KBD_scancode_buffer_idx] = scancode;
		KBD_scancode_buffer_idx++;
	}
	
	if(KBD_ascii_buffer_idx >= keyboard_buffer_size){
        KBD_ascii_buffer_idx = 0;
    }
	if(KBD_scancode_buffer_idx >= keyboard_buffer_size){
        KBD_scancode_buffer_idx = 0;
    }
}

char kbd_getChar(){
	char c = KBD_flags.key;
	KBD_flags.key = 0;
	return c;
}

char getc_blk(){
	printk("GetCBLK\n");
	uint32_t c = 0;
	while(c == 0){
		if(tasks[task_running_idx].window != &windows[window_selected]){
			continue;
		}
		c = kbd_getChar();
	}
	return (char) (c);
}
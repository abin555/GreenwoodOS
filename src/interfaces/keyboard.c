#include "keyboard.h"

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
	if(source != PS2_KBD) return;
	if(scancode){
		if(kbd_US[scancode] != 0){
			if(KBD_flags.ctrl && !KBD_flags.shift){
				print_serial("Check if Window Switch %c\n", kbd_US[scancode]);
				if(kbd_US[scancode] >= '1' && kbd_US[scancode] <= '9'){
					print_serial("Is Window Switch\n");
					int new_window = kbd_US[scancode] - '1';
					print_serial("Window %d\n", new_window);
					if(windows[new_window].active){
						window_selected = new_window;
						window_copy_buffer(&windows[window_selected]);
					}
				}
			}
			else if(KBD_flags.shift){
				keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = kbd_US_shift[scancode];
			}
			else{
				keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = kbd_US[scancode];
			}
			KBD_ascii_buffer_idx++;
			keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
		}
		else{
			switch(scancode){
				case 0x2a:
					KBD_flags.shift = true;
					break;
				case 0xaa:
					KBD_flags.shift = false;
					break;
				case 0x1D:
					KBD_flags.ctrl = true;
					break;
				case 0x9D:
					KBD_flags.ctrl = false;
					break;
				case 0xE0:
					KBD_flags.arrow = true;
					break;
				case 0xF0:
					if(KBD_flags.arrow && scancode & 0x80){
						KBD_flags.arrow = false;
					}
					break;
				case 0x4F:
					if(KBD_flags.arrow){
						print_serial("End!\n");
						for(int i = 0; i < MAX_TASKS; i++){
							if(tasks[i].window == &windows[window_selected]){
								stop_task(i);
							}
						}
						//reboot();
					}
					break;
				case 0x0E:
					keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 8;
					KBD_ascii_buffer_idx++;
					keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
					break;
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
	char retVal = '\0';
	if(KBD_last_key_idx != KBD_ascii_buffer_idx){
		int check_idx = ((int) KBD_ascii_buffer_idx) - 1;
		if(check_idx < 0){
			check_idx = keyboard_buffer_size - check_idx;
		}

		retVal = keyboard_ASCIIBuffer[check_idx];
		KBD_last_key_idx = KBD_ascii_buffer_idx;
	}
	return retVal;
}

char getc_blk(){
	char c = 0;
	while(c == 0 || tasks[task_running_idx].window != &windows[window_selected]){
		c = kbd_getChar();
	}
	return c;
}
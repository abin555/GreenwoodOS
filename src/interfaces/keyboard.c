#include "keyboard.h"
#include "vfs.h"
#include "sysfs.h"

uint32_t keyboard_buffer_size;
uint8_t *keyboard_KEYBuffer = NULL;
char *keyboard_ASCIIBuffer = NULL;

struct KBD_flags KBD_flags;

uint32_t KBD_scancode_buffer_idx;
uint32_t KBD_ascii_buffer_idx;
uint32_t KBD_last_key_idx;

char key_pressed_map[0xFF];

void kbd_init(uint32_t buffer_size){
	keyboard_buffer_size = buffer_size;
	keyboard_KEYBuffer = (uint8_t *) malloc(keyboard_buffer_size);
	memset(keyboard_KEYBuffer, 0, keyboard_buffer_size);
	keyboard_ASCIIBuffer = (char *) malloc(keyboard_buffer_size);
	memset(keyboard_ASCIIBuffer, 0, keyboard_buffer_size);
	print_serial("[KBD] buffer size: %d @ 0x%x 0x%x\n", keyboard_buffer_size, keyboard_ASCIIBuffer, keyboard_KEYBuffer);

	KBD_scancode_buffer_idx = 0;
	KBD_ascii_buffer_idx = 0;
	KBD_last_key_idx = 0;
	KBD_flags.shift = 0;
	KBD_flags.ctrl = 0;
	KBD_flags.arrow = 0;
	KBD_flags.release = 0;
	KBD_flags.backspace = 0;
	KBD_flags.special = 0;
	print_serial("[KBD] buffer size: %d\n", keyboard_buffer_size);
}

void kbd_callEventHandler(struct KBD_flags *flags){
	for(int i = 0; i < MAX_TASKS; i++){
		if(
			tasks[i].slot_active &&
			tasks[i].keyboard_event_handler != NULL && 
			tasks[i].window == &windows[window_selected]
		){
			//print_serial("Switching to program %d and calling keyboard handler at 0x%x, and keycode 0x%x (%c)\n", tasks[i].program_slot, (uint32_t) tasks[i].keyboard_event_handler, ascii, ascii);
			if(tasks[i].program_slot != -1)
				select_program(tasks[i].program_slot);
			tasks[i].keyboard_event_handler(flags);
			if(tasks[task_running_idx].program_slot != -1){
				select_program(tasks[task_running_idx].program_slot);
			}
		}
	}
}

void kbd_recieveScancode(uint8_t scancode, KBD_SOURCE source){
	//print_serial("[Keyboard Driver] Keyboard recieved scancode %x from %x\n", scancode, source);
	char justRelease = 0;
	char justSpecial = 0;
	keyboard_buffer_size = 255;
	if(source != PS2_KBD) return;
	if(keyboard_ASCIIBuffer == NULL || keyboard_KEYBuffer == NULL){
		print_serial("%d - 0x%x 0x%x\n", keyboard_buffer_size, keyboard_ASCIIBuffer, keyboard_KEYBuffer);
		keyboard_ASCIIBuffer = malloc(keyboard_buffer_size);
		keyboard_KEYBuffer = malloc(keyboard_buffer_size);
		return;
	}
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
						window_render_bar();
					}
				}
				else if(kbd_US[scancode] == '`'){
					task_lock = !task_lock;
				}
				else if(kbd_US[scancode] == 'a'){
					struct MouseButtons btns = {0, 0, 0};
					mouse_update(-1, 0, btns);
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
			*(key_pressed_map + KBD_flags.key) = 1;
			//print_serial("[KBD] Pressed %c\n", KBD_flags.key);
			fb_putChar(fb_width - 8, fb_height - 8, KBD_flags.key, 0xFFFFFF, 0x0);
			
			KBD_ascii_buffer_idx++;
			keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
			kbd_callEventHandler(&KBD_flags);
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
						//print_serial("End!\n");
						for(int i = 0; i < MAX_TASKS; i++){
							if(tasks[i].window == &windows[window_selected]){
								stop_task(i);
							}
						}
						//reboot();
					}
					break;
				case 0x6B://Left Arrow
					if(KBD_flags.special){
						KBD_flags.arrow = LEFT;
						//print_serial("Left Arrow\n");
						KBD_flags.key = 0x11;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0x11;
						KBD_ascii_buffer_idx++;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
						kbd_callEventHandler(&KBD_flags);
						if(KBD_flags.ctrl){
							mouse_update(-2, 0, (struct MouseButtons) {0, 0, 0});
						}
					}
					break;
				case 0x74://Right Arrow
					if(KBD_flags.special){
						KBD_flags.arrow = RIGHT;
						//print_serial("Right Arrow\n");
						KBD_flags.key = 0x12;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0x12;
						KBD_ascii_buffer_idx++;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
						kbd_callEventHandler(&KBD_flags);
						if(KBD_flags.ctrl){
							mouse_update(2, 0, (struct MouseButtons) {0, 0, 0});
						}
					}
					break;
				case 0x75://Up Arrow
					if(KBD_flags.special){
						KBD_flags.arrow = UP;
						//print_serial("Up Arrow\n");
						KBD_flags.key = 0x13;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0x13;
						KBD_ascii_buffer_idx++;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
						kbd_callEventHandler(&KBD_flags);
						if(KBD_flags.ctrl){
							mouse_update(0, 2, (struct MouseButtons) {0, 0, 0});
						}
					}
					break;
				case 0x72://Down Arrow
					if(KBD_flags.special){
						KBD_flags.arrow = DOWN;
						//print_serial("Down Arrow\n");
						KBD_flags.key = 0x14;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0x14;
						KBD_ascii_buffer_idx++;
						keyboard_ASCIIBuffer[KBD_ascii_buffer_idx] = 0;
						kbd_callEventHandler(&KBD_flags);
						if(KBD_flags.ctrl){
							mouse_update(0, -2, (struct MouseButtons) {0, 0, 0});
						}
					}
					break;
			}
			if(KBD_flags.release && !justRelease){
				char c;
				if(KBD_flags.shift){
					c = kbd_US_shift[scancode];
				}
				else{
					c = kbd_US[scancode];
				}
				*(key_pressed_map + c) = 0;
				KBD_flags.release = false;
				KBD_flags.key = 0;
				if(KBD_flags.arrow != 0){
					KBD_flags.arrow = 0;
				}
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
	return keyboard_ASCIIBuffer[KBD_ascii_buffer_idx-1];
}

char getc_blk(){
	uint32_t c = 0;
	//return (char) (c);
	uint32_t current_ascii_idx = KBD_ascii_buffer_idx;
	while(1){
		if(tasks[task_running_idx].window != &windows[window_selected]){
			continue;
		}
		if(current_ascii_idx != KBD_ascii_buffer_idx){
			return keyboard_ASCIIBuffer[KBD_ascii_buffer_idx-1];
		}
	}
	return (char) (c);
}

int getArrow(){
	return KBD_flags.arrow;
}

void *kbd_createCDEV(){
	print_serial("[KBD] Generating CDEV\n");
	struct SysFS_Chardev *kbd_cdev = sysfs_createCharDevice(
		(char *) &key_pressed_map,
		sizeof(key_pressed_map),
		CDEV_READ
	);
	return (void *) kbd_cdev;
}

int kbdTxt_read(void *dev, void *buf, int roffset, int nbytes, int *head){
	struct SysFS_Chardev *kbddev = dev;
	int n = snprintf(
		kbddev->buf,
		kbddev->buf_size,
		"%d\n",
		KBD_ascii_buffer_idx
	);
	for(uint32_t i = 0; i < keyboard_buffer_size && n < kbddev->buf_size-1; i++){
		if(keyboard_ASCIIBuffer[i] != '\0'){
			kbddev->buf[n++] = keyboard_ASCIIBuffer[i];
		}
	}
	kbddev->buf[n] = '\0';
	int step = roffset;
	int i;
	for(i = 0; i < nbytes && step < kbddev->buf_size && i < n; i++){
		((char *) buf)[i] = kbddev->buf[step++];
	}
	*head += i;
	return i;
}

void *kbdTxt_createCDEV(){
	print_serial("[KBD] Generating Text CDEV\n");
	struct SysFS_Chardev *kbd_cdev = sysfs_createCharDevice(
		malloc(keyboard_buffer_size + 20),
		keyboard_buffer_size + 20,
		CDEV_READ
	);
	sysfs_setCallbacks(
		kbd_cdev,
		NULL,
		NULL,
		NULL,
		kbdTxt_read
	);
	return (void *) kbd_cdev;
}
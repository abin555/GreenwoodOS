#include "libc.h"

struct WINDOW *window;
uint32_t *win_buf;

int main(int argc, char *argv[]){
    print("TETRIS Start!\n");
    window = window_open("Tetris!");
    set_schedule(ONFOCUS);
    win_buf = window->backbuffer;

    int i = 500;
    while(1){
        win_buf[i++] = 0xDEADBEEF;

        window_update();
        char input = getc();
        if(input == 'Q') break;
    }

    window_close(window);
    print("TETRIS Exit.\n");
}

/*

Blocks:

**
**

*
*
*
*

*
***
  
  *
***

 * 
***

**
 **

 **
**


*/



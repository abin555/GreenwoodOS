#include "libc.h"

int main(int argc, char **argv){
    struct Viewport *vp = viewport_open(100, 100, "Test Program Viewport!");
    while(!vp->minimized){

    }
    viewport_close(vp);
}
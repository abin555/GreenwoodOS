#include "libc.h"

int main(int argc, char **argv){
    struct ViewportFunctions *vp_funcs = viewport_get_funcs();
    struct Viewport *vp = vp_funcs->open(100, 100, "Test Program Viewport!");
    while(!vp->minimized){

    }
    vp_funcs->close(vp);
}
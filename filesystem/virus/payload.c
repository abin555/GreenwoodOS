#include "libc.h"
char msg[] = "Hello There";
void Virus(){
    dprint("[Virus] Hello There!\n");
    struct FEATURE_INFO framebuffer = getKernelFeature(FEAT_FRAMEBUFFER);
    struct ViewportFunctions *viewport_fn = viewport_get_funcs();
    struct Viewport *virus_vp = viewport_fn->open(sizeof(msg)*8, 8, "Virus");
    (virus_vp-1)->title="I HATE MATH!";
    viewport_fn->close(virus_vp);
    while(1){
        (virus_vp-1)->title[2]++;
    }
}
#include "libc.h"

void memset(void *mem, char v, int size);

int main(int argc, char **argv){
    print("Bork\n");
    struct FEATURE_INFO font = getKernelFeature(FEAT_FONT);
    srand(4321);
    for(int i = 0; i < font.size * 5; i++){
        ((char *) font.addr)[i] = rand();
    }
    exec(argv[0], argc, argv);
    asm("hlt");
    return 0;
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}
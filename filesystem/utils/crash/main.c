#include <stdio.h>

int main(int argc, char **argv){
    printf("Crash-Tastic!\n");
    int *poke = (int *) 0x3F00000;
    for(int i = 0; i < 1000; i++){
        poke[i*0x100] = -28 *i;
    }
    return 0;
}
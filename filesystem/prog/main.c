struct WINDOW{
    unsigned int *backbuffer;
    char *name;
    char active;
    unsigned int width;
    unsigned int height;
    char copyPromptOnly;
};

int main(int argc, char **argv){
	register unsigned int eax asm("eax");
    register unsigned int ebx asm("ebx");
    register unsigned int ecx asm("ecx");
    register unsigned int edx asm("edx");    
    ebx = (unsigned int) "Test Application";
    eax = 0x01;
    asm("int 0x80");
    struct WINDOW *win = (void *) eax;
    
    edx = argc;
    ecx = 0x0;
    ebx = 0x0;
    eax = 0x04;
    asm("int 0x80");
    eax = 0x03;
    asm("int 0x80");

    unsigned int *fb = (unsigned int *) win->backbuffer;
	for(int i = 0; i < 10000; i++){
		fb[i+0x10000] = 0xFFFFFF - i;
	}

    while(1){

    }    
}
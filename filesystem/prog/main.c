int main(){
	register unsigned int eax asm("eax");
    register unsigned int ebx asm("ebx");
    register unsigned int ecx asm("ecx");
    register unsigned int edx asm("edx");
	unsigned int offset = eax;
	unsigned int *fb = (unsigned int *) 0xFD000000;
	for(int i = 0; i < 10000; i++){
		fb[i+offset] = 0xFFFFFF - i;
	}
    
    ebx = (unsigned int) "Program";
    eax = 0x01;
    asm("int 0x80");
    void *win = (void *) eax;
    edx = 'A';
    ecx = 0x0;
    ebx = 0x0;
    eax = 0x04;
    asm("int 0x80");
    eax = 0x03;
    asm("int 0x80");
    while(1){

    }    
}
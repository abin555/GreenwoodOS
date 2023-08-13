int main(){
	register unsigned int eax asm("eax");
	unsigned int offset = eax;
	unsigned int *fb = (unsigned int *) 0xFD000000;
	for(int i = 0; i < 10000; i++){
		fb[i+offset] = 0xFFFFFF - i;
		for(int x = 0; x < 100000; x++){
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }
	}
}
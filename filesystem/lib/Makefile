CFLAGS = -m32 -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -masm=intel

build:
	i386-elf-gcc -c $(CFLAGS) src/crt0.c -o crt0.o
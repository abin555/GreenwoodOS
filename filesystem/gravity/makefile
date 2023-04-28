CFLAGS = -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../libc/link.ld -melf_i386

all:
	$(CC) $(CFLAGS) gravity.c -o gravity.o
	$(CC) $(CFLAGS) ../libc/libc.c -o libc.o
	$(CC) $(CFLAGS) ../libc/math.c -o math.o
	ld $(LDFLAGS) gravity.o libc.o math.o -o grav.elf
	objcopy -O binary --only-section=.text grav.elf grav.exe
	rm gravity.o
	rm libc.o
	rm grav.elf


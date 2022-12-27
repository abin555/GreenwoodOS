CFLAGS = -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../libc/link.ld -melf_i386

all:
	$(CC) $(CFLAGS) main.c -o main.o
	$(CC) $(CFLAGS) ../libc/libc.c -o libc.o
	$(CC) $(CFLAGS) ../libc/math.c -o math.o
	ld $(LDFLAGS) main.o libc.o math.o -o cube.elf
	objcopy -O binary --only-section=.text cube.elf cube.exe
	rm main.o
	rm libc.o
	rm math.o
	rm cube.elf


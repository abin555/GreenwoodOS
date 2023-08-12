CFLAGS = -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ./link.ld -melf_i386

all:
	$(CC) $(CFLAGS) main.c -o main.o
	ld $(LDFLAGS) main.o -o test.elf
	objcopy -O binary --only-section=.text test.elf test.exe
	rm main.o
	rm test.elf
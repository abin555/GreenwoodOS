CFLAGS = -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../libc/link.ld -melf_i386

all:
	$(CC) $(CFLAGS) main.c -o main.o
	$(CC) $(CFLAGS) ../libc/libc.c -o libc.o
	ld $(LDFLAGS) main.o libc.o -o tetris.elf
	objcopy -O binary --only-section=.text tetris.elf tetris.exe
	objdump -d tetris.elf > tetris.s
	rm main.o
	rm libc.o
	rm tetris.elf
	make -C ../../ make_fs


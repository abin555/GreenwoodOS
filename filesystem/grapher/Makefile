CFLAGS = -I ../OS/libc/ -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../OS/libc/link.ld -melf_i386

FILES = main.c
OBJ = grapher.o
LIBS = ../OS/libc/libc.o
START = ../OS/libc/start.o
PROG = grapher

all:
	make -C ../OS/libc
	$(CC) $(CFLAGS) $(FILES) -o $(OBJ)
	ld $(LDFLAGS) $(START) $(OBJ) $(LIBS) -o $(PROG).elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe
	rm $(OBJ)
	rm $(PROG).elf
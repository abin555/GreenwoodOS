CFLAGS = -I ../OS/libc/ -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../OS/libc/link.ld -melf_i386

FILES = virus.c
OBJ = virus.o
LIBS = ../OS/libc/libc.o
START = ../OS/libc/start.o
PROG = virus

all:
	make -C ../OS/libc
	$(CC) $(CFLAGS) $(FILES) -o $(OBJ)
	$(CC) $(CFLAGS) payload.c -o payload.o
	ld $(LDFLAGS) $(START) $(OBJ) $(LIBS) -o $(PROG).elf
	ld -T ./payload.ld -melf_i386 payload.o $(LIBS) -o payload.elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe
	objcopy -O binary --only-section=.text payload.elf payload.exe
	rm $(OBJ)
	rm $(PROG).elf
	rm payload.elf
	rm payload.o
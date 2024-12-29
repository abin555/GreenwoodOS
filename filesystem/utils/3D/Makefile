CFLAGS = -I ../../OS/libc/ -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../../OS/libc/link.ld -melf_i386

FILES = main.c
OBJ = 3D.o
LIBS = ../../OS/libc/libc.o
PROG = 3D

all:
	make -C ../../OS/libc
	$(CC) $(CFLAGS) -gstabs $(FILES) -o $(OBJ)
	ld $(LDFLAGS) $(OBJ) $(LIBS) -o $(PROG).elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe

	$(CC) $(CFLAGS) -g mainvp.c -o 3Dvp.o
	ld $(LDFLAGS) 3Dvp.o $(LIBS) -o 3Dvp.elf
	objcopy -O binary --only-section=.text 3Dvp.elf 3Dvp.exe
#	rm $(OBJ)
#	rm $(PROG).elf
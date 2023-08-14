CFLAGS = -I ../libc/ -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ASFLAGS = -f elf
LDFLAGS = -T ../libc/link.ld -melf_i386

FILES = main.c
OBJ = echo.o
LIBS = ../libc/libc.o
PROG = echo

all:
	make -C ../libc
	$(CC) $(CFLAGS) $(FILES) -o $(OBJ)
	ld $(LDFLAGS) $(OBJ) $(LIBS) -o $(PROG).elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe
	rm $(OBJ)
	rm $(PROG).elf
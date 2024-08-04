CC = g++
CFLAGS = -g -I ../OS/libc/ -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector -fPIC
ASFLAGS = -f elf
LDFLAGS = -T ./link.ld -melf_i386

OBJ = main.o window.o renderer.o utilities.o terrain.o game.o
LIBS = ../OS/libc/libc.o
PROG = terra

all:
	make -C ../OS/libc
	$(CC) $(CFLAGS) main.cpp -o main.o
	$(CC) $(CFLAGS) window.cpp -o window.o
	$(CC) $(CFLAGS) renderer.cpp -o renderer.o
	$(CC) $(CFLAGS) utilities.cpp -o utilities.o
	$(CC) $(CFLAGS) -fpermissive terrain.cpp -o terrain.o
	$(CC) $(CFLAGS) game.cpp -o game.o
	ld $(LDFLAGS) $(OBJ) $(LIBS) -o $(PROG).elf
	objcopy -O binary $(PROG).elf $(PROG).exe
#	rm $(OBJ)
#	rm $(PROG).elf
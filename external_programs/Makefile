OBJECTS = \
	./test.o

CC = gcc
CFLAGS = -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -Wall -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -T link.ld -melf_i386

all:
	$(AS) $(ASFLAGS) test.s -o test.o
	ld $(LDFLAGS) test.o -o test
	$(CC) $(CFLAGS) external.c -o external.o
	ld $(LDFLAGS) external.o -o external
	objdump -d external > external.txt
	objdump -d test > test.txt


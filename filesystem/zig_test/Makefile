LDFLAGS = -T ../OS/libc/link.ld -melf_i386
PROG = zig_test

build:
	zig build-obj main.zig -target x86-freestanding
	gcc support.c -g -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector -o support.o
	ld $(LDFLAGS) ../OS/libc/start.o ./main.o ../OS/libc/libc.o ./support.o -o $(PROG).elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe
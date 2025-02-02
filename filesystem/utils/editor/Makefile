CFLAGS = -I . -g -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector
ZIGFLAGS = -target x86-freestanding
LDFLAGS = -T ../../OS/libc/link.ld -melf_i386
START = ../../OS/libc/start.o
LIBC = ../../OS/libc/libc.o

CC = gcc
AS = nasm
ZIGC = zig

PROG = zig_ed

SRCS += main.o
SRCS += rt.o
SRCS += support.o

libc_header = ../../OS/libc/libc.h

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@
%.o: %.zig
	$(ZIGC) build-obj $< $(ZIGFLAGS)

build:# $(SRCS)
	#zig translate-c $(libc_header) > libc.zig
	#ld $(LDFLAGS) $(START) $(SRCS) $(LIBC) -o $(PROG).elf
	#objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe
	#rm *.o *.elf
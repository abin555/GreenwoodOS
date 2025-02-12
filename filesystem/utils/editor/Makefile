CC = gcc
CFLAGS = -g -m32 -c -O0 -m32 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -masm=intel
INCLUDE_PATH = -I ../../OS/libc/ -I./libgif
LIBS = ../../OS/libc/libc.o
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -T ../../OS/libc/link.ld -melf_i386 -g
PROG = editor

SRCS += main.o
SRCS += gui.o
SRCS += memory.o

START = start.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

build: $(SRCS) $(START)
	ld $(LDFLAGS) $(START) $(SRCS) $(LIBS) -o $(PROG).elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe
	objcopy --only-keep-debug $(PROG).elf $(PROG).sym

clean:
	rm $(SRCS)
	rm $(START)
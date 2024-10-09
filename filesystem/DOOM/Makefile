CC = clang
CFLAGS = -g -m32 -no-pie -c -O0 -nostdlib -nodefaultlibs -nolibc -masm=intel -fno-asynchronous-unwind-tables -fcf-protection=none -fno-builtin -fno-stack-protector -fPIC
INCLUDE_PATH = -I ../OS/libc/ -I./include/
LIBS = ../OS/libc/libc.o
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -T ./link.ld -melf_i386
PROG = DOOM

SRCS += doom_gwos.o


START = start.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

build: $(SRCS) $(START)
	ld $(LDFLAGS) $(START) $(SRCS) $(LIBS) -o $(PROG).elf
	objcopy -O binary --only-section=.text $(PROG).elf $(PROG).exe

clean:
	rm $(SRCS)
	rm $(START)
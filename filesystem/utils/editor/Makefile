CC = i386-gwos-gcc
CFLAGS = -g -c -O0 -nostdlib -nodefaultlibs -masm=intel
INCLUDE_PATH = -I ../../OS/libc/
LIBS = ../../OS/libc/libc.o
AS = nasm
ASFLAGS = -f elf
LDFLAGS = 
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
#	i386-gwos-gcc $(START) $(SRCS) $(LIBS) -o $(PROG).elf
#	i386-gwos-objcopy -O binary $(PROG).elf $(PROG).exe
#	i386-gwos-objcopy --only-keep-debug $(PROG).elf $(PROG).sym

clean:
	rm $(SRCS)
	rm $(START)
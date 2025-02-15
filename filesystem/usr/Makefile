CC = i386-elf-gcc
AS = i386-elf-as
CFLAGS = -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -masm=intel
INCLUDE_PATH = -I ../OS/libc/ -I./include
LIBS = ../OS/libc/libc.o

SRCS += xassert.o
SRCS += crt0.o

output_lib := cstdlib

%.o : ./src/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o : ./src/*.S
	$(AS) $< -o $@

build: $(SRCS)
	echo "Build?"
	ar -rcs $(output_lib).a $(SRCS) $(LIBS)

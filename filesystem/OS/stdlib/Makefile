CC = i386-elf-gcc
CFLAGS = -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -masm=intel
INCLUDE_PATH = -I ../libc/ -I./include
LIBS = ../libc/libc.o

SRCS += xassert.o

output_lib := cstdlib

%.o : ./src/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@

build: $(SRCS)
	echo "Build?"
	ar -rcs $(output_lib).a $(SRCS) $(LIBS)
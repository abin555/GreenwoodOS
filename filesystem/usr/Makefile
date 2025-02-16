CC = i386-elf-gcc
AS = i386-elf-as
CFLAGS = -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -masm=intel
INCLUDE_PATH = -I../OS/libc/ -I./include
LIBS = ../OS/libc/libc.o

C_FILES := ctype.c errno.c stdlib.c xassert.c
SRCS := $(patsubst %.c,%.o,$(C_FILES))

output_lib := libc

%.o : ./src/%.c
	$(CC) $(INCLUDE_PATH) -c $(CFLAGS) $< -o $@
%.o : ./src/*.s
	$(AS) $< -o $@

build: clean $(SRCS)
	echo "Build?"
	ar -rcs lib/$(output_lib).a $(SRCS) $(LIBS)

clean:
	-rm $(SRCS) $(output_lib).a
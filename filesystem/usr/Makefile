CC = i386-elf-gcc
AS = i386-elf-as
CFLAGS = -m32 -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -fPIC -masm=intel
INCLUDE_PATH = -I../OS/libc/ -I./include
LIBS =

C_FILES := $(wildcard ./src/*.c)
C_FILES += $(wildcard ./src/sys/*.c)
SRCS := $(patsubst %.c,%.o,$(C_FILES))

output_lib := libc

%.o : %.c
	$(CC) $(INCLUDE_PATH) -c $(CFLAGS) $< -o $@
%.o : *.s
	$(AS) $< -o $@

build: clean $(SRCS)
	echo "Build?"
	ar -rcs lib/$(output_lib).a $(SRCS) $(LIBS)

clean:
	-rm $(SRCS) lib/$(output_lib).a
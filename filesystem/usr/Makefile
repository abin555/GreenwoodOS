CC = i386-elf-gcc
AS = i386-elf-as
CFLAGS = -m32 -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -fPIC -masm=intel
INCLUDE_PATH = -I../OS/libc/ -I./include
LIBS =

LIBC_FILES := $(wildcard ./libc_src/*.c)
LIBC_FILES += $(wildcard ./libc_src/sys/*.c)
LIBC_SRCS := $(patsubst %.c,%.o,$(LIBC_FILES))

output_libc := libc

%.o : %.c
	$(CC) $(INCLUDE_PATH) -c $(CFLAGS) $< -o $@
%.o : *.s
	$(AS) $< -o $@

build_libc: clean $(LIBC_SRCS)
	echo "Build?"
	ar -rcs lib/$(output_libc).a $(LIBC_SRCS)

build_math:
	make -C ./fdlibm all
	cp ./fdlibm/libm.a ./lib/libm.a
	cp ./fdlibm/fdlibm.h ./include/math.h

build: build_libc build_math
	echo "Built"

clean:
	-rm $(LIBC_SRCS) lib/$(output_libc).a
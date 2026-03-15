CC = i386-elf-gcc
AS = i386-elf-as
CFLAGS = -m32 -c -O0 -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -fPIC -masm=intel
INCLUDE_PATH = -I../OS/libc/ -I./include
LIBS =

LIBC_FILES := $(wildcard ./src/libc_src/*.c)
LIBC_FILES += $(wildcard ./src/libc_src/sys/*.c)
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
	make -C ./src/fdlibm all
	cp ./src/fdlibm/libm.a ./lib/libm.a
	cp ./src/fdlibm/fdlibm.h ./include/math.h

build_gwoslib:
	make -C ./src/libgwos_src build
	cp ./src/libgwos_src/libgwos.a ./lib/libgwos.a

build_exec:
	make -C ./src/libexec build
	cp ./src/libexec/libexec.a ./lib/libexec.a

build: build_libc build_math build_gwoslib build_exec
	echo "Built"

clean:
	-rm $(LIBC_SRCS) lib/$(output_libc).a
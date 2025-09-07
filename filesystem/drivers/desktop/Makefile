CFLAGS := -fPIC -Wl,-q
CC := i386-gwos-gcc

build:
	$(CC) $(CFLAGS) main.c viewport.c bitmap.c icon.c gfx.c hooks.c -o desktop.elf

default: build
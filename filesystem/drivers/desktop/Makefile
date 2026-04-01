CFLAGS := -fPIC -Wl,-q
CC := i386-gwos-gcc

build:
	$(CC) $(CFLAGS) main.c bitmap.c hooks.c gfx.c viewport.c -lexec -o desktop.elf

default: build
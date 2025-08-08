CFLAGS := -fPIC -pie -g
CC := i386-gwos-gcc


loader:
	$(CC) loader.c -o loader.elf

service:
	$(CC) $(CFLAGS) service.c -o service.elf

all: loader service

default: all


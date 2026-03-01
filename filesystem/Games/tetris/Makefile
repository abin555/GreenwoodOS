CC = i386-gwos-gcc
PROG = tetris

SRCS += main.c
SRCS += tetris.c
SRCS += screen.c

build:
	$(CC) -O0 -g $(SRCS) -o $(PROG).elf
	
clean:
	rm $(SRCS)
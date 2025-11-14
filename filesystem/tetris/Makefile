CC = i386-gwos-gcc
PROG = tetris

SRCS += main.c
SRCS += block.c
SRCS += game.c

build:
	$(CC) -O0 -g $(SRCS) -o $(PROG).elf
	
clean:
	rm $(SRCS)
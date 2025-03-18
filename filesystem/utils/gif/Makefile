CC = i386-gwos-gcc
CFLAGS = -O0 -std=c17
INCLUDE_PATH = -I./libgif
PROG = gif

SRCS += main.c
#SRCS += utils.c
SRCS += libgif/dgif_lib.c
SRCS += libgif/gif_err.c
#SRCS += libgif/gif_hash.c
SRCS += libgif/gifalloc.c
SRCS += libgif/openbsd-reallocarray.c

build:
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(SRCS) -o $(PROG).elf

clean:
	rm $(SRCS)
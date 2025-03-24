#ifndef INTERNAL_STDIO_H
#define INTERNAL_STDIO_H

struct FILE {
	enum {
		FILE_console,
		FILE_fd
	} file_type;
	union {
		struct CONSOLE *console;
		int fd;
	};
};

extern struct FILE **fileListing;

#endif
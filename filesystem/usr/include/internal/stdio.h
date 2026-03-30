#ifndef INTERNAL_STDIO_H
#define INTERNAL_STDIO_H

struct FILE {
	enum {
		FILE_fd
	} file_type;
	union {
		int fd;
	};
	int ungetc_buf;
};

extern struct FILE **fileListing;

#endif
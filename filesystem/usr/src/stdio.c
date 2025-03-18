#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/console.h>

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

#define MAX_FILE_LISTING 10
struct FILE **fileListing;
int openFiles;

FILE *internal_createFileConsole(struct CONSOLE *console){
	FILE *file = malloc(sizeof(FILE));
	file->file_type = FILE_console;
	file->console = console;
	return file;
}

FILE *internal_createFileFD(int fd){
	FILE *file = malloc(sizeof(FILE));
	file->file_type = FILE_fd;
	file->fd = fd;
	return file;
}

void init_stdio(){
	fileListing = malloc(sizeof(FILE *) * MAX_FILE_LISTING);
	fileListing[0] = NULL;
	fileListing[1] = internal_createFileConsole(console_get_current());
	fileListing[2] = fileListing[1];
	openFiles = 3;
	puts("Stdio setup\n");
}

int fprintf(FILE *file, const char *fmt, ...){
	if(file == NULL) return 0;
	if(file->file_type == FILE_console){
		console_print(file->console, (char *) fmt);
	}
	else{
		write(file->fd, (char *) fmt, strlen(fmt)+1);
	}
}

int printf(const char *fmt, ...){
	return fprintf(stdout, fmt);
}

int fputs(const char *s, FILE *file){
	if(file == NULL) return 0;
	if(file->file_type == FILE_console){
		console_print(file->console, (char *) s);
	}
	else{
		write(file->fd, (char *) s, strlen(s)+1);
	}
}

int puts(const char *s){
	return fputs(s, stdout);
}

FILE *fopen(const char *pathname, const char *mode){

}

int fclose(FILE *stream){
	
}
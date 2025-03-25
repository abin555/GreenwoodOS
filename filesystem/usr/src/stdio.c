#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <internal/stdio.h>

#define MAX_FILE_LISTING 10
struct FILE **fileListing;
int openFiles;

FILE *internal_createFileFD(int fd){
	FILE *file = malloc(sizeof(FILE));
	file->file_type = FILE_fd;
	file->fd = fd;
	return file;
}

void init_stdio(){
	fileListing = malloc(sizeof(FILE *) * MAX_FILE_LISTING);
	memset(fileListing, 0, sizeof(fileListing));
	fileListing[0] = NULL;
	fileListing[1] = internal_createFileFD(open("/-/dev/console", O_WRITE));
	fileListing[2] = fileListing[1];
	openFiles = 3;
}

int fprintf(FILE *file, const char *fmt, ...){
	va_list listpd;
	va_start(listpd, fmt);
	char s[40];
	int num;
	char *s_arg;
	char *b;
	if(file == NULL) return 0;
	if(file->file_type == FILE_fd){
		//write(file->fd, (char *) fmt, strlen(fmt)+1);
		while(*fmt != '\0'){
			if(*fmt != '%'){
				write(file->fd, (void *) fmt, 1);
				fmt++;
			}
			else{
				fmt++;
				switch(*fmt){
					case '%':
						write(file->fd, "%", 1);
						break;
					case 'd':
						num = va_arg(listpd, int);
						b = itoa(num, s, 10);
						write(file->fd, b, strlen(b)+1);
						break;
					case 's':
						s_arg = va_arg(listpd, char *);
						write(file->fd, s_arg, strlen(s_arg)+1);
						break;
					case 'x':
						num = va_arg(listpd, int);
						b = itoa(num, s, 16);
						write(file->fd, b, strlen(b)+1);
						break;
				}
				fmt++;
			}
		}
	}
}

int printf(const char *fmt, ...){
	va_list listpd;
	va_start(listpd, fmt);
	char s[40];
	int num;
	char *s_arg;
	char *b;
	if(stdout == NULL) return 0;
	if(stdout->file_type == FILE_fd){
		while(*fmt != '\0'){
			if(*fmt != '%'){
				write(stdout->fd, (void *) fmt, 1);
				fmt++;
			}
			else{
				fmt++;
				switch(*fmt){
					case '%':
						write(stdout->fd, "%", 1);
						break;
					case 'd':
						num = va_arg(listpd, int);
						b = itoa(num, s, 10);
						write(stdout->fd, b, strlen(b)+1);
						break;
					case 's':
						s_arg = va_arg(listpd, char *);
						write(stdout->fd, s_arg, strlen(s_arg)+1);
						break;
					case 'x':
						num = va_arg(listpd, int);
						b = itoa(num, s, 16);
						write(stdout->fd, b, strlen(b)+1);
						break;
				}
				fmt++;
			}
		}
	}
}

int fputs(const char *s, FILE *file){
	if(file == NULL) return 0;
	if(file->file_type == FILE_fd){
		write(file->fd, (char *) s, strlen(s)+1);
	}
}

int puts(const char *s){
	return fputs(s, stdout);
}

FILE *fopen(const char *pathname, const char *mode){
	int i;
	for(i = 0; i < MAX_FILE_LISTING; i++){
		if(fileListing[i] == NULL) break;
	}
	if(i == MAX_FILE_LISTING) return NULL;
	fileListing[i] = internal_createFileFD(open((char *) pathname, O_READ | O_WRITE));
	return fileListing[i];
}

int fclose(FILE *stream){
	for(int i = 0; i < MAX_FILE_LISTING; i++){
		if(fileListing[i] == stream){
			fileListing[i] = NULL;
			break;
		}
	}
	return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream){
	if(stream == NULL || ptr == NULL) return 0;
	for(int i = 0; i < size; i++){
		if(stream->file_type == FILE_fd){
			write(stream->fd, (void *) ptr, n);
		}
		else{

		}
	}
}
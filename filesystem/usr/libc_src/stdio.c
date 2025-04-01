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
	int fd = open((char *) pathname, O_READ | O_WRITE);
	if(fd == -1){
		return NULL;
	}
	fileListing[i] = internal_createFileFD(fd);
	return fileListing[i];
}

int fclose(FILE *stream){
	for(int i = 0; i < MAX_FILE_LISTING; i++){
		if(fileListing[i] == stream){
			if(fileListing[i]->file_type == FILE_fd){
				close(fileListing[i]->fd);
			}
			fileListing[i] = NULL;
			break;
		}
	}
	return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream){
	if(stream == NULL || ptr == NULL) return 0;
	int n_wrote = 0;
	for(int i = 0; i < n; i++){
		if(stream->file_type == FILE_fd){
			n_wrote += write(stream->fd, (void *) ptr, size);
		}
		else{

		}
	}
	return n_wrote;
}

size_t fread(void *ptr, size_t size, size_t n, FILE *stream){
	if(stream == NULL || ptr == NULL) return 0;
	int n_read = 0;
	for(int i = 0; i < n; i++){
		if(stream->file_type == FILE_fd){
			n_read += read(stream->fd, (void *) ptr, size);
		}
		else{

		}
	}
	return n_read;
}

int feof(FILE *stream){
	int head, end;
	if(stream == NULL) return 1;
	if(stream->file_type == FILE_fd){
		head = lseek(stream->fd, 0, SEEK_CUR);
		end = lseek(stream->fd, 0, SEEK_END);
		lseek(stream->fd, head, SEEK_SET);
		if(head >= end) return 1;
		return 0;
	}
}

long ftell(FILE *stream){
	if(stream == NULL) return 0;
	if(stream->file_type == FILE_fd)
		return lseek(stream->fd, 0, SEEK_CUR);
	return 0;
}

int fseek(FILE *stream, long offset, int whence){
	if(stream == NULL) return -1;
	if(stream->file_type == FILE_fd){
		lseek(stream->fd, (int) offset, whence);
		return 0;
	}
	return -1;
}

FILE *freopen(const char *pathname, const char *mode, FILE *stream){
	int matching_idx = -1;
	for(int i = 0; i < openFiles; i++){
		if(fileListing[i] == stream){
			matching_idx = i;
			break;
		}
	}
	if(matching_idx != -1){
		fclose(stream);
		fileListing[matching_idx] = internal_createFileFD(open((char *) pathname, O_READ | O_WRITE));
		return fileListing[matching_idx];
	}
	return fopen(pathname, mode);
}
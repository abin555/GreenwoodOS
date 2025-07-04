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

int vfprintf(FILE *file, const char *fmt, va_list listpd){
	char s[40];
	int num;
	float f;
	double d;
	char *s_arg;
	char *b;
	int ntok = 0;
	if(file == NULL) return 0;
	while(*fmt != '\0'){
		if(*fmt != '%'){
			ntok += fwrite((void *) fmt, 1, 1, file);
			fmt++;
		}
		else{
			fmt++;
			switch(*fmt){
				case '%':
					ntok += fwrite("%", 1, 1, file);
					break;
				case 'd':
					num = va_arg(listpd, int);
					b = itoa(num, s, 10);
					ntok += fwrite(b, strlen(b)+1, 1, file);
					break;
				case 's':
					s_arg = va_arg(listpd, char *);
					ntok += fwrite(s_arg, strlen(s_arg)+1, 1, file);
					break;
				case 'x':
					num = va_arg(listpd, int);
					b = itoa(num, s, 16);
					ntok += fwrite(b, strlen(b)+1, 1, file);
					break;
				case 'F':
					break;
			}
			fmt++;
		}
	}
	return ntok;
}

int fprintf(FILE *file, const char *fmt, ...){
	va_list listpd;
	va_start(listpd, fmt);
	return vfprintf(file, fmt, listpd);
}

int printf(const char *fmt, ...){
	va_list listpd;
	va_start(listpd, fmt);
	if(stdout == NULL) return 0;
	return vfprintf(stdout, fmt, listpd);
}

int fputs(const char *s, FILE *file){
	if(file == NULL) return 0;
	fwrite(s, strlen(s), 1, file);
}

int puts(const char *s){
	int n = fputs(s, stdout);
	fwrite("\n", 1, 1, stdout);
	return n+1;
}

FILE *fopen(const char *pathname, const char *mode){
	int i;
	for(i = 0; i < MAX_FILE_LISTING; i++){
		if(fileListing[i] == NULL) break;
	}
	if(i == MAX_FILE_LISTING) return NULL;
	int fd = open((char *) pathname, O_READ | O_WRITE);
	if(fd == -1){
		printf("fopen fail\n");
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
			int nstep = write(stream->fd, (void *) ptr, size);
			n_wrote += nstep;
			/*
			if(nstep < size){
				if(feof(stream)){
					int csize = ftell(stream);
					int missing = size*n - n_wrote;
					ftruncate(stream->fd, csize + missing);
				}
			}
			*/
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
			read(stream->fd, (void *) (ptr+n_read*size), size);
			n_read++;
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

int fputc(int c, FILE *stream){
    char cc = (char) c;
    return fwrite(&cc, 1, 1, stream);    
}

int putc(int c, FILE *stream){
    return fputc(c, stream);
}

int putchar(int c){
    return putc(c, stdout);
}

int snprintf(char *str, size_t size, const char *format, ...){
	va_list listpd;
	va_start(listpd, format);
	if(str == NULL || format == NULL || size <= 0) return 0;
	char wbuf[100];
	int idx = 0;
	char *s;
	size_t n = 0;
	while(*format != '\0' && n < size){
		if(*format != '%'){
			str[idx++] = *format++;
			n++;
		}
		else{
			format++;
			switch(*format){
				case 's':{
					s = va_arg(listpd, char *);
					int l = strlen(s);
					for(int i = 0; i < l && n < size; i++){
						str[idx++] = s[i];
						n++;
					}
					break;
				}
				case 'd':{
					int val = va_arg(listpd, int);
					s = itoa(val, wbuf, 10);
					int l = strlen(s);
					for(int i = 0; i < l && n < size; i++){
						str[idx++] = s[i];
						n++;
					}
					break;
				}
				case 'x':{
					int val = va_arg(listpd, int);
					s = itoa(val, wbuf, 16);
					int l = strlen(s);
					for(int i = 0; i < l && n < size; i++){
						str[idx++] = s[i];
						n++;
					}
					break;
				}
			}
			format++;
		}
	}
	return n;
}
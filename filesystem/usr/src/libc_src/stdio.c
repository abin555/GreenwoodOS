#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <internal/stdio.h>
#include <ctype.h>

#define MAX_FILE_LISTING 10
struct FILE **fileListing;
int openFiles;

FILE *internal_createFileFD(int fd){
	FILE *file = malloc(sizeof(FILE));
	file->file_type = FILE_fd;
	file->fd = fd;
	file->ungetc_buf = -1;
	return file;
}

void init_stdio(){
	fileListing = malloc(sizeof(FILE *) * MAX_FILE_LISTING);
	memset(fileListing, 0, sizeof(fileListing));
	fileListing[0] = internal_createFileFD(0);
	fileListing[1] = internal_createFileFD(1);
	fileListing[2] = fileListing[1];
	openFiles = 3;
}

int vfprintf(FILE *file, const char *fmt, va_list listpd){
	char s[40];
	int num;
	unsigned int unum;
	float f;
	double d;
	char *s_arg;
	char *b;
	char c;
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
					unum = va_arg(listpd, unsigned int);
					b = utoa(num, s, 16);
					ntok += fwrite(b, strlen(b)+1, 1, file);
					break;
				case 'F':
					break;
				case 'c':
					c = (char) va_arg(listpd, int);
					s[0] = c;
					ntok += fwrite(s, 1, 1, file);
					break;
			}
			fmt++;
		}
	}
	return ntok;
}

int vsprintf(const char *str, const char *fmt, va_list listpd){
	char s[40];
	int num;
	unsigned int unum;
	float f;
	double d;
	char *s_arg;
	char *b;
	char c;
	int ntok = 0;
	int idx = 0;
	if(str == NULL) return 0;
	char *buf = str;
	while(*fmt != '\0'){
		if(*fmt != '%'){
			buf[idx++] = *fmt;
			ntok += 1;
			fmt++;
		}
		else{
			fmt++;
			switch(*fmt){
				case '%':
					//ntok += fwrite("%", 1, 1, file);
					buf[idx++] = '%';
					ntok++;
					break;
				case 'd':
					num = va_arg(listpd, int);
					b = itoa(num, s, 10);
					//ntok += fwrite(b, strlen(b)+1, 1, file);
					for(int i = 0; i < strlen(b); i++){
						buf[idx++] = b[i];
						ntok++;
					}
					break;
				case 's':
					s_arg = va_arg(listpd, char *);
					//ntok += fwrite(s_arg, strlen(s_arg)+1, 1, file);
					for(int i = 0; i < strlen(s_arg); i++){
						buf[idx++] = s_arg[i];
						ntok++;
					}
					break;
				case 'x':
					unum = va_arg(listpd, unsigned int);
					b = utoa(num, s, 16);
					//ntok += fwrite(b, strlen(b)+1, 1, file);
					for(int i = 0; i < strlen(b); i++){
						buf[idx++] = b[i];
						ntok++;
					}
					break;
				case 'F':
					break;
				case 'c':
					c = (char) va_arg(listpd, int);
					s[0] = c;
					//ntok += fwrite(s, 1, 1, file);
					buf[idx++] = c;
					ntok++;
					break;
			}
			fmt++;
		}
	}
	buf[idx++] = '\0';
	return ntok;
}

int sprintf(char *str, const char *format, ...){
	va_list listpd;
	va_start(listpd, format);
	return vsprintf(str, format, listpd);
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
				case 'c':{
					char val = (char) va_arg(listpd, int);
					str[idx++] = val;
					n++;
					break;
				}
			}
			format++;
		}
	}
	return n;
}

int fgetc(FILE *stream){
	if (stream->ungetc_buf != -1) {
        int c = stream->ungetc_buf;
        stream->ungetc_buf = -1;
        return c;
    }
	char buf[1];
	int n = fread(buf, sizeof(buf), 1, stream);
	if(n == 0) return EOF;
	if(buf[0] == '\0') return EOF;
	return buf[0];
}

char *fgets(char s[], int size, FILE *stream){
	if(s == NULL || stream == NULL) return NULL;
	char c = 0;
	int n = 0;
	for(int i = 0; i < size-1; i++){
		c = fgetc(stream);
		if(c == EOF) break;
		s[n++] = c;
		if(c == '\n') break;
	}
	return s;
}

int remove(const char *path){
	return 0;
}

int fileno(FILE *stream){
	return stream->fd;
}

void perror(const char *s){
	if(s == NULL) s = "n/a";
	printf("%s: PERROR\n", s);
}

int fflush(FILE *stream){
	return 0;
}

int ferror(FILE *stream){
	return 0;
}

int ungetc(int c, FILE *stream)
{
    if (c == EOF)
        return EOF;

    if (stream->ungetc_buf != -1)
        return EOF;   /* buffer already full, only 1 pushback guaranteed by standard */

    stream->ungetc_buf = (unsigned char)c;
    return (unsigned char)c;
}

int sscanf(const char *str, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    const char *s = str;
    int matched = 0;

    for (const char *f = fmt; *f != '\0'; f++) {

        /* Whitespace in format matches any amount of whitespace in input */
        if (isspace(*f)) {
            while (isspace(*s))
                s++;
            continue;
        }

        /* Non-conversion literal character must match exactly */
        if (*f != '%') {
            if (*s != *f)
                goto done;
            s++;
            continue;
        }

        /* Step over '%' */
        f++;

        /* Parse optional width */
        int width = 0;
        while (isdigit(*f)) {
            width = width * 10 + (*f - '0');
            f++;
        }

        switch (*f) {

        /* Signed decimal integer: %d */
        case 'd': {
            while (isspace(*s))
                s++;

            if (*s == '\0')
                goto done;

            int neg = 0;
            if (*s == '-') { neg = 1; s++; }
            else if (*s == '+') { s++; }

            if (!isdigit(*s))
                goto done;

            int value = 0;
            int digits = 0;
            while (isdigit(*s) && (width == 0 || digits < width)) {
                value = value * 10 + (*s - '0');
                s++;
                digits++;
            }

            *va_arg(ap, int *) = neg ? -value : value;
            matched++;
            break;
        }

        /* Unsigned decimal integer: %u */
        case 'u': {
            while (isspace(*s))
                s++;

            if (*s == '\0')
                goto done;

            if (!isdigit(*s))
                goto done;

            unsigned int value = 0;
            int digits = 0;
            while (isdigit(*s) && (width == 0 || digits < width)) {
                value = value * 10 + (*s - '0');
                s++;
                digits++;
            }

            *va_arg(ap, unsigned int *) = value;
            matched++;
            break;
        }

        /* Hexadecimal integer: %x */
        case 'x':
        case 'X': {
            while (isspace(*s))
                s++;

            if (*s == '\0')
                goto done;

            /* Skip optional 0x prefix */
            if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
                s += 2;

            unsigned int value = 0;
            int digits = 0;
            while ((width == 0 || digits < width)) {
                unsigned int nibble;
                if (*s >= '0' && *s <= '9')      nibble = *s - '0';
                else if (*s >= 'a' && *s <= 'f') nibble = *s - 'a' + 10;
                else if (*s >= 'A' && *s <= 'F') nibble = *s - 'A' + 10;
                else break;
                value = value * 16 + nibble;
                s++;
                digits++;
            }

            if (digits == 0)
                goto done;

            *va_arg(ap, unsigned int *) = value;
            matched++;
            break;
        }

        /* String (non-whitespace sequence): %s */
        case 's': {
            while (isspace(*s))
                s++;

            if (*s == '\0')
                goto done;

            char *out = va_arg(ap, char *);
            int chars = 0;
            while (*s != '\0' && !isspace(*s) && (width == 0 || chars < width)) {
                *out++ = *s++;
                chars++;
            }

            if (chars == 0)
                goto done;

            *out = '\0';
            matched++;
            break;
        }

        /* Single character: %c */
        case 'c': {
            /* %c does NOT skip whitespace */
            if (*s == '\0')
                goto done;

            int count = (width == 0) ? 1 : width;
            char *out = va_arg(ap, char *);
            for (int i = 0; i < count && *s != '\0'; i++)
                *out++ = *s++;

            matched++;
            break;
        }

        /* Literal percent: %% */
        case '%': {
            if (*s != '%')
                goto done;
            s++;
            break;
        }

        default:
            goto done;
        }
    }

done:
    va_end(ap);
    return matched;
}
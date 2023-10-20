#ifndef LIBC_H
#define LIBC_H

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef char bool;
#define true 1;
#define false 0;
#define NULL 0

struct WINDOW {
	uint32_t *backbuffer;
	char name[21];
	bool active;
	uint32_t width;
	uint32_t height;
	bool copyOnPromptOnly;
};	

typedef enum {
    ALWAYS = 1,
    ONFOCUS = 2,
    NOCHILD = 3
} ScheduleType;

struct CONSOLE{
	struct WINDOW *window;
	uint32_t width;
	uint32_t height;
	uint32_t buf_size;
	char *buf;
	bool active;
	int last_cursor;
	int cursor;

	uint32_t Line;
	uint32_t Start;
	uint32_t LinePlace;
	uint32_t LastLine;
	struct{
		uint32_t fg;
		uint32_t bg;
	} color;
};

struct FILE;

int strcmp(const char *s1, const char *s2);
struct WINDOW *window_open(char *name);
void window_close(struct WINDOW *window);
void window_update();
void drawChar(uint32_t x, uint32_t y, char c);
char getc();
void exec(char *filename, int argc, char **argv);
void set_schedule(ScheduleType type);
void print(char *str);
void print_arg(char *str, uint32_t arg);
struct CONSOLE *console_open();
void console_close();
void *kmalloc(uint32_t size);
struct FILE *fopen(char *file);
void fclose(struct FILE *file);
char fgetc(struct FILE *file);
int fsize(struct FILE *file);
int fcopy(struct FILE *file, char *buf, int buf_size);
int changeDirectory(char *change);
char *getDirectory();
void printDirectoryContents(char *path);
int fexists(char *path);

typedef enum {
	FEAT_FONT = 0x01,
	FEAT_KEYBOARD = 0x02,
	FEAT_PAGETABLE = 0x03,
	FEAT_TASKTABLE = 0x04,
	FEAT_PROGRAMBASE = 0x05
} KERNEL_FEATURE;

struct FEATURE_INFO{
	void *addr;
	uint32_t size;
};

struct FEATURE_INFO getKernelFeature(KERNEL_FEATURE feature);

void fseek(struct FILE *file, int idx);
void fputc(struct FILE *file, char c);
void fmkdir(char *path);
void fmkfile(char *path, int size);
int getArrowKey();

void srand(uint32_t seed);
int rand();

#endif
#ifndef LIBC_H
#define LIBC_H
#ifdef __cplusplus
extern "C"{
#endif

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

#ifndef __cplusplus
#define bool char
#define true 1
#define false 0
#endif

#define NULL 0

struct IVec2{
    int x;
    int y;
};

struct MouseButtons{
    uint8_t right : 1;
    uint8_t left : 1;
    uint8_t middle : 1;
};

struct MouseStatus{
    struct IVec2 pos;
    struct MouseButtons buttons;
    struct IVec2 lastDelta;
};

struct PCSpeaker_Handle{
    void (*play)(uint32_t);
    void (*mute)(void);
    void (*beep)(void);
};

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
    NOCHILD = 3,
	NEVER = 4
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

struct File_Info{
	void *drive;
	uint32_t sector;
	uint32_t size;
};

struct FILE{
	struct File_Info info;
	int head;
};

int strcmp(const char *s1, const char *s2);
struct WINDOW *window_open(char *name, uint32_t copyOnPrompt);
void window_close(struct WINDOW *window);
void window_update();
void drawChar(uint32_t x, uint32_t y, char c);
char getc();
void exec(char *filename, int argc, char **argv);
void set_schedule(ScheduleType type);
void print(char *str);
void print_arg(char *str, uint32_t arg);
void print_float(char *str, float arg);
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
	FEAT_PROGRAMBASE = 0x05,
	FEAT_FRAMEBUFFER = 0x06,
	FEAT_TIMER = 0x07,
	FEAT_KEYPRESSMAP = 0x08,

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
int fextend(struct FILE *file, uint32_t amount);
int getArrowKey();

void srand(uint32_t seed);
int rand();
void exit(int code);

void *requestRegion(unsigned int bytes);

void addKbdEventHandler(void (*handler)(unsigned char));
void *requestRegionAt(unsigned int bytes, unsigned int addr);
void attachTimerCallback(unsigned int timer, void *callback);
void dprint(char *msg);
void start_manual_task(void *addr, char *name);
struct MouseStatus *getMouse();
struct PCSpeaker_Handle *getPCSpeaker();
uint32_t *getTimerTickHandle();
void addMouseEventHandler(void (*handler)(void));
void freeRegion(void *addr, uint32_t size);
void addEndCallback(void (*handler)(void));

#ifndef SYS_VP_H

struct Location {
    int x;
    int y;
    int w;
    int h;
};

typedef enum {
    VP_FOCUSED,
    VP_UNFOCUSED,
    VP_KEY,
    VP_EXIT,
    VP_MINIMIZE,
    VP_MAXIMIZE,
    VP_CLICK
} VIEWPORT_EVENT_TYPE;

struct Viewport {
    struct Location loc;
    struct Location oldLoc;
    uint32_t *frontbuf;
    uint32_t *backbuf;
    uint32_t buf_size;
    uint8_t resizeable;
    bool minimized;
    bool open;
    int minimized_w;
    int minimized_h;
    char *title;
    int owner_program_slot;
    int owner_task_id;
    char ascii;

    void (*event_handler)(struct Viewport *, VIEWPORT_EVENT_TYPE);
	bool click_events_enabled;
    bool transparent;
};

struct ViewportFunctions {
    struct Viewport *(*open)(int, int, char*);
    void (*close)(struct Viewport*);
    void (*set_buffer)(struct Viewport* viewport, uint32_t* buf, uint32_t size);
    void (*copy)(struct Viewport*);
    void (*add_event_handler)(struct Viewport *, void (*)(struct Viewport *, VIEWPORT_EVENT_TYPE));
    void (*drawChar)(struct Viewport *, int, int, char, uint32_t, uint32_t);
    char (*getc)(struct Viewport *);
};

struct ViewportFunctions *viewport_get_funcs();
struct CONSOLE *console_open_vp(struct Viewport *vp);

#endif

struct RealTimeClock {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned int msec;
};

struct RealTimeClock *get_rtc();

struct DirectoryEntry{
	char filename[50];
	int name_len;
	uint32_t type;
};

struct DirectoryListing{
	char *directory_path;
	int directory_path_len;
	struct DirectoryEntry *entries;
	int num_entries;
};

struct DirectoryListing getDirectoryListing(char *path);
void print_serial(char *str);
void task_lock(int state);
void write_serial(char c);

void yield();

typedef enum {
    O_READ = 0b1,
    O_WRITE = 0b10
} OPENFLAGS;

int open(char *path, OPENFLAGS flags);
void close(int fd);
int read(int fd, void *buf, int nbytes);
int write(int fd, void *buf, int nbytes);
int lseek(int fd, int off, int whence);
int creat(char *path);
int dup(int fd);
int fork();

#ifdef __cplusplus
}
#endif

#endif


extern void init_stdlib();
extern void init_stdio();
extern void _init();
extern void main(int argc, char **argv);
extern void clean_stdlib();
extern int sys_vp_initialized;
extern void exit(int);

extern void puts(const char *s);

void _start(int argc, char **argv){
    sys_vp_initialized = 0;
    init_stdlib();
    init_stdio();
    //_init();

    main(argc, argv);

    clean_stdlib();
    //exit(0);
    return;
}

extern void init_stdlib();
extern void init_stdio();
extern void _init();
extern void main(int argc, char **argv);
extern void clean_stdlib();
extern int sys_vp_initialized;

void _start(int argc, char **argv){
    sys_vp_initialized = 0;
    init_stdlib();
    init_stdio();
    
    _init();

    main(argc, argv);

    clean_stdlib();
    return;
}
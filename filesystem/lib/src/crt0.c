
extern void init_stdlib();
extern void init_stdio();
extern void _init();
extern void main(int argc, char **argv);
extern void clean_stdlib();

void _start(int argc, char **argv){
    init_stdlib();
    init_stdio();
    
    _init();

    main(argc, argv);

    clean_stdlib();
    return;
}
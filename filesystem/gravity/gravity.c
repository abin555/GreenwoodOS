#include "../libc/libc.h"


//Window Forward Definitions
#define width 500
#define height 500
uint32_t window_buffer[width*height];
void *window_ptr;
void window_event_handler(WINDOW_EVENT e);
void clear_window(uint32_t color);

//Simulator Forward Definitions
bool run_sim;
struct position{
    int x;
    int y;
};
struct velocity{
    int Vx;
    int Vy;
};
struct object{
    struct position pos;
    struct velocity vel;
    int mass;
};
#define num_objects 2
struct object objects[num_objects];
void initialize_object(struct object* obj, int x, int y, int Vx, int Vy, int mass);

//Rendering Forward Definitions
void render_sim();

int main(){
    print("Gravity Simulator!\n");
    get_keycode_next();
    window_ptr = open_window(width, height, window_buffer);
    add_window_event_handler(window_ptr, window_event_handler);
    clear_window(0);
    print("Gravity Simulator Window Online\n");
    run_sim = 1;

    initialize_object(&objects[0], 250, 250, 0, 0, 10);
    initialize_object(&objects[1], 50, 50 , 0, 0, 10);

    while(run_sim){
        uint8_t keycode = get_keycode_next();
        switch(keycode){
            case 0x81:
                run_sim = 0;
                print("Sim Exit\n");
                break;
        }

        render_sim();
    }
    close_window(window_ptr);    
}

void clear_window(uint32_t color){
    for(uint32_t i = 0; i < width*height; i++){
        window_buffer[i] = color;
    }
}

void window_event_handler(WINDOW_EVENT e){
    switch(e){
        case close:
            print("Close Window");
            run_sim = 0;
            break;
    }
}

void initialize_object(struct object* obj, int x, int y, int Vx, int Vy, int mass){
    obj->pos.x = x;
    obj->pos.y = y;
    obj->vel.Vx = Vx;
    obj->vel.Vy = Vy;
    obj->mass = mass;
}

void render_sim(){
    for(int i = 0; i < num_objects; i++){
        window_buffer[
            objects[i].pos.y * width + objects[i].pos.x
        ] = 0xFFFFFF;
    }
}
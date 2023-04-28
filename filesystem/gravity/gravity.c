#include "../libc/libc.h"
#include "../libc/math.h"

//Window Forward Definitions
#define width 500
#define height 500
uint32_t window_buffer[width*height];
void *window_ptr;
void window_event_handler(WINDOW_EVENT e);
void clear_window(uint32_t color);

//Simulator Forward Definitions
#define G_const -5
bool run_sim;
struct position{
    float x;
    float y;
};
struct velocity{
    float Vx;
    float Vy;
};
struct object{
    struct position pos;
    struct velocity vel;
    int mass;
};
struct force{
    float Fx;
    float Fy;
};

#define num_objects 3
struct object objects[num_objects];
void initialize_object(struct object* obj, float x, float y, float Vx, float Vy, int mass);
struct force calculateNetGravForce(int obj_idx);
float distance(struct object *obj1, struct object *obj2);

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

    initialize_object(&objects[0], 250, 50, -1, 0, 1);
    initialize_object(&objects[1], 50, 250 , 0, 0, 1);
    initialize_object(&objects[2], 250, 250 , 0, 0, 1000);

    int sim_step = 0;

    while(run_sim){
        
        uint8_t keycode = get_keycode_next();
        switch(keycode){
            case 0x81:
                run_sim = 0;
                print("Sim Exit\n");
                break;
        }
        
        if(sim_step == 1000){
            //print("Physics Step\n");
            for(int i = 0; i < num_objects; i++){
                struct force obj_netForce = calculateNetGravForce(i);
                float xA = obj_netForce.Fx / objects[i].mass;
                float yA = obj_netForce.Fy / objects[i].mass;
                objects[i].vel.Vx += xA;
                objects[i].vel.Vy += yA;
            }
            for(int i = 0; i < num_objects; i++){
                objects[i].pos.x += objects[i].vel.Vx;
                objects[i].pos.y += objects[i].vel.Vy;
            }        
            render_sim();
            sim_step = 0;
        }
        sim_step++;
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

void initialize_object(struct object* obj, float x, float y, float Vx, float Vy, int mass){
    obj->pos.x = x;
    obj->pos.y = y;
    obj->vel.Vx = Vx;
    obj->vel.Vy = Vy;
    obj->mass = mass;
}

struct force calculateNetGravForce(int obj_idx){
    struct force ForceNet;
    for(int i = 0; i < num_objects; i++){
        if(i == obj_idx) continue;
        float R = distance(&objects[obj_idx], &objects[i]);
        float F = (G_const * (objects[obj_idx].mass + objects[i].mass)) / (R*R);
        float distX = objects[i].pos.x - objects[obj_idx].pos.x;
        float distY = objects[i].pos.y - objects[obj_idx].pos.y;
        //printf("Obj %d to %d distX: %f distY: %f\n", obj_idx, i, distX, distY);
        double theta = acos(distX / R);
        //printf("Theta: %f\n", theta * (180 / 3.1415));

        float Fx = F*sin(theta);
        float Fy = F*cos(theta);

        if(Fx < 0) Fx *= -1;
        if(Fy < 0) Fy *= -1;
        if(distX < 0) Fx *= -1;
        if(distY < 0) Fy *= -1;

        ForceNet.Fx += Fx;
        ForceNet.Fy += Fy;
    }
    return ForceNet;
}

void render_sim(){
    for(int i = 0; i < num_objects; i++){
        if(
            objects[i].pos.y >= 0 && objects[i].pos.y < height &&
            objects[i].pos.x >= 0 && objects[i].pos.x < width
        ){
            window_buffer[
                (int) objects[i].pos.y * width + (int) objects[i].pos.x
            ] = 0xFFFFFF;
        }
    }
}

float distance(struct object *obj1, struct object *obj2){
    float xPart = (obj2->pos.x - obj1->pos.x);
    float yPart = (obj2->pos.y - obj1->pos.y);
    return sqrt(
        (xPart * xPart) + (yPart * yPart)
    );
}
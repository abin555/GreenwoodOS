#include <sys/task.h>
#include "graphics.h"
#include "object.h"
#include "physics.h"

int main(int argc, char **argv){
    struct Window *win = window_init(200, 200, "Gravity");
    
    Object *o1 = object_create(1.0, 10.0, (Vec2) {0.0, 0.0}, (Vec2) {0.0, 0.0});
    Object *o2 = object_create(0.01, 2.0 , (Vec2) {-0.25f, 0.0f}, (Vec2) {0.0, 1.2f});
    Object *o3 = object_create(0.01, 2.0 , (Vec2) {0.5f, 0.0f}, (Vec2) {0.0, -0.75f});
    int num_objects = 3;
    double timeStep = 0.01f;
    Object *objects[3] = {
        o1,
        o2,
        o3
    };
    while(1){
        Object *largestObject = objects[0];
        for(int i = 0; i < num_objects; i++){
            if(objects[i]->mass > largestObject->mass){
                largestObject = objects[i];
            }
        }

        for(int i = 0; i < num_objects; i++){
            for(int j = 0; j < num_objects; j++){
                if(i == j) continue;

                Force fAB = physics_getGravForce(objects[i], objects[j]);
                Force fBA = physics_getGravForce(objects[j], objects[i]);
                object_applyForce(objects[i], fAB);
                object_applyForce(objects[j], fBA);
            }
        }
        for(int i = 0; i < num_objects; i++){
            if(objects[i] != largestObject) 
                object_update(objects[i], timeStep);
        }

        

        window_clear(win);

        for(int i = 0; i < num_objects; i++){
            gfx_drawObject(
                win,
                objects[i],
                false,
                false
            );
        }

        window_update(win);
        yield();
    }
}
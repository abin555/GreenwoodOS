#include <stdlib.h>
#include <stdint.h>
#include "object.h"
#include "physics.h"

Object *object_create(double m, double r, Vec2 pos, Vec2 vel){
    Object *o = malloc(sizeof(Object));
    o->mass = m;
    o->radius = r;
    o->position = pos;
    o->velocity = vel;
    o->acceleration = (Vec2) {0.0, 0.0};
    return o;
}
void object_applyForce(Object *object, Force f){
    if(object == NULL) return;
    Vec2 a = {
        f.direction.x * (f.magnitude / object->mass),
        f.direction.y * (f.magnitude / object->mass)
    };
    object->acceleration.x += a.x;
    object->acceleration.y += a.y;
}
void object_update(Object *object, double t){
    object->velocity = physics_accelToVel(object->acceleration, object->velocity, t);
    object->position = physics_velToPos(object->velocity, object->position, t);
    object->acceleration = (Vec2) {
        0.0f,
        0.0f
    };
}
void object_clean(Object *object){

}
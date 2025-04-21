#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "physics.h"

#define P_G 1

double physics_getDistance(Object *a, Object *b){
    return(
        sqrt(
            pow(b->position.x - a->position.x, 2) + pow(b->position.y - a->position.y, 2)
        )
    );
}

Force physics_getGravForce(Object *a, Object *b){
    Force f;
    double distance = physics_getDistance(a, b);
    f.magnitude = P_G * (a->mass * b->mass) / pow(distance, 2.0);
    f.direction = (Vec2) {
        b->position.x - a->position.x,
        b->position.y - a->position.y
    };
    return f;
}

Vec2 physics_accelToVel(Vec2 a, Vec2 v, double t){
    Vec2 result = {
        v.x + a.x * t,
        v.y + a.y * t
    };
    return result;
}

Vec2 physics_velToPos(Vec2 v, Vec2 p, double t){
    Vec2 result = {
        p.x + v.x * t,
        p.y + v.y * t
    };
    return result;
}

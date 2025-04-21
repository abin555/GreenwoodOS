#ifndef OBJECT_H
#define OBJECT_H

#include "tools.h"

typedef struct Object {
    double mass;
    double radius;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
} Object;

Object *object_create(double m, double r, Vec2 pos, Vec2 vel);
void object_applyForce(Object *object, Force f);
void object_update(Object *object, double t);
void object_clean(Object *object);

#endif
#ifndef PHYSICS_H
#define PHYSICS_H

#include "tools.h"
#include "object.h"

double physics_getDistance(Object *a, Object *b);
Force physics_getGravForce(Object *a, Object *b);
Vec2 physics_accelToVel(Vec2 a, Vec2 v, double t);
Vec2 physics_velToPos(Vec2 v, Vec2 p, double t);

#endif
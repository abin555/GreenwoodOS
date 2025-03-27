#ifndef BOID_H
#define BOID_H
#include <sys/vp.h>
#include "gfx.h"

#define M_PI 3.14159265f

struct Vector2 {
    float x;
    float y;
};

typedef struct Vector2 Vector2;

struct Boid {
    Vector2 origin;
    float rotation;
    Vector2 *positions;
    Vector2 velocity;
    float angularVelocity;
    double lastUpdate;
};

typedef struct Boid Boid;

Boid *newBoid(Vector2 origin, Vector2 velocity, float rotation, float angularVelocity);
void updateBoid(Boid *boid, Boid **flock, int flockSize);
void rotateBoid(Boid *boid, float theta);
void drawBoid(Boid *boid, struct Viewport *vp);

#endif
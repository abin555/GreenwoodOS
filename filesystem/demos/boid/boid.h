#ifndef BOID_H
#define BOID_H
#include <sys/vp.h>
#include "gfx.h"

#define M_PI 3.14159265f

struct Boid {
    Vector2 origin;
    float rotation;
    Vector2 positions[3];
    Vector2 velocity;
    float angularVelocity;
    double lastUpdate;
};

typedef struct Boid Boid;

Boid *newBoid(Vector2 origin, Vector2 velocity, float rotation, float angularVelocity);
void updateBoid(Boid *boid, Boid **flock, int flockSize, double now);
void rotateBoid(Boid *boid, float theta);
void drawBoid(Boid *boid, struct Viewport *vp);

#endif
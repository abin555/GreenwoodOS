#ifndef TOOLS_H
#define TOOLS_H

typedef struct {
    double x;
    double y;
} Vec2;

typedef struct {
    int x;
    int y;
} IVec2;

#define Vec2ToIVec(A) ((IVec2) {(int) A.x, (int) A.y})
#define normalize(min, max, v, a, b) ((((b - a) * (v - min)) / (max - min)) + a)

typedef struct {
    double magnitude;
    Vec2 direction;
} Force;

#endif
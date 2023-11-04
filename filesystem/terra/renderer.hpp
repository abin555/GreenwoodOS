#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "libc.h"
#include "application.hpp"
#include "utilities.hpp"

typedef struct {
  float m[4][4];
} Matrix4x4;

struct Vector3{
    float x;
    float y;
    float z;
};

typedef Vector3 Vertex;

struct TriangleExpl{
    Vertex v0;
    Vertex v1;
    Vertex v2;
};

struct TriangleRef{
    Vertex *v0;
    Vertex *v1;
    Vertex *v2;
};

struct TriangleIdx{
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
};

class Mesh{
    private:

    public:

    unsigned int numVertex;
    unsigned int numTriangles;
    Vertex *vertex;
    struct TriangleIdx *triangles;

    Vertex *findVertex(float x, float y, float z);
    Vertex *matchVertex(Vertex vert);
    void rotateX(float theta);
    void rotateY(float theta);
    void rotateZ(float theta);
    void translate(Vector3 offset);
    void loadObj(char *filename);
};

class Object{
    private:

    public:
    Mesh *masterMesh;
    Mesh *localMesh;
    Vector3 worldRotation;
    Vector3 worldPosition;
    Object(char *filename);
    Object(Mesh *masterMesh);
    Object();
    void cloneMaster(Mesh *original);
    void copyMesh();
    void transform();
};

struct Camera{
    Vector3 position;
    Vector3 rotation;
};

typedef unsigned int ZBuffType;

class World{
    private:
    Window *window;
    Camera cam;

    Vector3 light;

    int numObjects;
    int maxObjects;
    Object **objects;


    float fNear;
    float fFar;
    float fFov;
    float fAspectRatio;
    float fFovRad;
    Matrix4x4 ProjectionMatrix;

    public:
    ZBuffType *ZBuff;
    bool ZBuffering;
    bool FillFaces;
    bool DrawOutlines;
    World(Window *window, int maxObjects);
    World();
    void render();
    Camera *getCamera();
    void addObject(Object *object);
    struct TriangleExpl projectTriangle(TriangleExpl triangle);
    struct TriangleExpl cameraTranslate(TriangleExpl triangle);
    struct TriangleExpl cameraRotate(TriangleExpl triangle);
    void prepareRotationMatrices(Matrix4x4 *RotX, Matrix4x4 *RotY, Matrix4x4 *RotZ, Vector3 rot);
};

void multiplyMatrixVector(Vector3 *vi, Vector3 *vo, Matrix4x4 *m);
struct TriangleExpl triangleIdxToExpl(Mesh *mesh, struct TriangleIdx *tri);
Vector3 calculateNormal(TriangleExpl triangle);

void drawTriangle(TriangleExpl triangle, uint32_t color);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);

void printTriangle(TriangleExpl tri);
void fillTriangle(World *world, TriangleExpl triangle, TriangleExpl *unprojected, uint32_t color);

bool operator==(Vector3 &a, Vector3 &b);

#endif
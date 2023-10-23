#ifndef _3D_types_H
#define _3D_types_H

typedef struct {
  float x;
  float y;
} Vector2;

typedef struct {
  float x;
  float y;
  float z;
} Vector3;

typedef Vector3 Vertex;

typedef struct {
  float m[4][4];
} Matrix4x4;

typedef struct {
  Vertex *v0;
  Vertex *v1;
  Vertex *v2;
} TriangleRef;

typedef struct {
  int numVertex;
  Vertex *vertices;
  int numTriangle;
  TriangleRef *triangles;
} Object; 

typedef struct {
  Vertex v0;
  Vertex v1;
  Vertex v2;
} TriangleExpl;

#endif
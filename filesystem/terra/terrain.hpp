#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "application.hpp"
#include "utilities.hpp"
#include "renderer.hpp"

#define CHUNK_WIDTH 8
#define CHUNK_HEIGHT 32

typedef enum {
    none = 0b0,
    top = 0b000001,
    bottom = 0b000010,
    left = 0b000100,
    right = 0b001000,
    front = 0b010000,
    back = 0b100000
} BlockFaces;

class Chunk{
    private:
    
    public:
    uint8_t blocks[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_HEIGHT];
    
    Chunk();
    BlockFaces calculateFacesVisible(int x, int y, int z);
    uint32_t getColor(int x, int y, int z);
    void *getBlocksAddr();
};

class Terrain{
    private:
    int maxVertex;
    int maxTriangle;
    Mesh terrainMesh;
    Object terrainObject;
    
    public:
    Mesh *getMesh();
    Object *getObject();
    Vertex *addVertex(Vertex vert);
    void addSquareFace(BlockFaces face, float x, float y, float z, uint32_t color);
    void generateMesh(Chunk *chunk);
    Terrain(int maxVertex, int maxTriangle);
    Terrain();

};

#endif
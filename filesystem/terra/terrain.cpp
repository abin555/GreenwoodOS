#include "terrain.hpp"

Chunk::Chunk(){
    for(int z = 0; z < CHUNK_HEIGHT; z++){
        for(int x = 0; x < CHUNK_WIDTH; x++){
            for(int y = 0; y < CHUNK_WIDTH; y++){
                if(z < 3) this->blocks[x][y][z] = 1;
                else if(z == 3) this->blocks[x][y][z] = 2;
                else this->blocks[x][y][z] = 0;
            }
        }
    }
    this->blocks[1][1][5] = 1;
}

void *Chunk::getBlocksAddr(){
    return &this->blocks;
}

/*
x
/\
|      front
|     _______
|     |     |
|left | T/B | right
|     |_____|
|      back
*-------------->y
z



*/

BlockFaces Chunk::calculateFacesVisible(int x, int y, int z){
    BlockFaces faces = none;
    if(this->blocks[x][y][z] == 0) return faces;
    
    if(x == 0) faces |= back;
    if(x == CHUNK_WIDTH-1) faces |= front;

    if(x != 0 && this->blocks[x-1][y][z] == 0) faces |= back;
    if(x != CHUNK_WIDTH-1 && this->blocks[x+1][y][z] == 0) faces |= front;

    if(y == 0) faces |= left;
    if(y == CHUNK_WIDTH-1) faces |= right;

    if(y != 0 && this->blocks[x][y-1][z] == 0) faces |= left;
    if(y != CHUNK_WIDTH-1 && this->blocks[x][y+1][z] == 0) faces |= right;

    if(z == 0) faces |= bottom;
    if(z == CHUNK_HEIGHT-1) faces |= top;

    if(z != 0 && this->blocks[x][y][z-1] == 0) faces |= bottom;
    if(z != CHUNK_HEIGHT-1 && this->blocks[x][y][z+1] == 0) faces |= top;
    
    return faces;
}

uint32_t Chunk::getColor(int x, int y, int z){
    if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_WIDTH || z < 0 || z >= CHUNK_HEIGHT) return 0;
    switch(this->blocks[x][y][z]){
        case 0:
            return 0;
            break;
        case 1:
            return 0x606060;
        case 2:
            return 0x10AA10;
        default:
            return 0xFFFFFF;
    }
}

Terrain::Terrain(){
    return;
}

Terrain::Terrain(int maxVertex, int maxTriangle){
    this->maxVertex = maxVertex;
    this->maxTriangle = maxTriangle;
    this->terrainMesh.numVertex = 0;
    this->terrainMesh.numTriangles = 0;
    this->terrainMesh.vertex = (Vertex *) sys.malloc(sizeof(Vertex) * maxVertex);
    this->terrainMesh.triangles = (struct TriangleIdx *) sys.malloc(sizeof(struct TriangleIdx) * maxTriangle);
    
    this->terrainObject = Object();
    this->terrainObject.masterMesh = &this->terrainMesh;
    this->terrainObject.localMesh = (Mesh *) sys.malloc(sizeof(Mesh));
    this->terrainObject.localMesh->numVertex = 0;
    this->terrainObject.localMesh->numTriangles = 0;

    this->terrainObject.localMesh->vertex = (Vertex *) sys.malloc(sizeof(Vertex) * maxVertex);
    this->terrainObject.localMesh->triangles = (struct TriangleIdx *) sys.malloc(sizeof(struct TriangleIdx) * maxTriangle);
}

Object *Terrain::getObject(){
    return &this->terrainObject;
}

Mesh *Terrain::getMesh(){
    return &this->terrainMesh;
}

void Terrain::generateMesh(Chunk *chunk){
    this->terrainMesh.numVertex = 0;
    this->terrainMesh.numTriangles = 0;
    memset(this->terrainMesh.vertex, 0, sizeof(Vertex) * this->maxVertex);
    memset(this->terrainMesh.triangles, 0, sizeof(struct TriangleIdx) * this->maxTriangle);
    for(int z = 0; z < CHUNK_HEIGHT; z++){
        for(int x = 0; x < CHUNK_WIDTH; x++){
            for(int y = 0; y < CHUNK_WIDTH; y++){
                BlockFaces faces = chunk->calculateFacesVisible(x, y, z);
                uint32_t color = chunk->getColor(x, y, z);
                //print_arg("Face: %x\n", (uint32_t) faces);
                if(faces & top){
                    this->addSquareFace(top, x, y, z, color);
                }
                if(faces & front){
                    this->addSquareFace(front, x, y, z, color);
                }
                if(faces & back){
                    this->addSquareFace(back, x, y, z, color);
                }
                if(faces & left){
                    this->addSquareFace(left, x, y, z, color);
                }
                if(faces & bottom){
                    this->addSquareFace(bottom, x, y, z, color);
                }
                if(faces & right){
                    this->addSquareFace(right, x, y, z, color);
                }
            }
        }
    }
    
    this->terrainObject.localMesh->numVertex = this->terrainMesh.numVertex;
    this->terrainObject.localMesh->numTriangles = this->terrainMesh.numTriangles;

    //print_arg("Terrain has %d Verticies and ", this->terrainMesh.numVertex);
    //print_arg("%d faces\n", this->terrainMesh.numTriangles);

}

Vertex *Terrain::addVertex(Vertex vert){
    Vertex *vertAddr = NULL;
    vertAddr = this->terrainMesh.matchVertex(vert);
    if(vertAddr != NULL){
        //print("Vertex Match!\n");
        return vertAddr;
    }
    if(this->terrainMesh.numVertex < this->maxVertex){
        this->terrainMesh.vertex[this->terrainMesh.numVertex] = vert;
        vertAddr = &this->terrainMesh.vertex[this->terrainMesh.numVertex];
        this->terrainMesh.numVertex++;
        //print_arg("Terrain mesh now has %d verticies @0x", this->terrainMesh.numVertex);
        //print_arg("%x\n", (uint32_t) vertAddr);
    }
    return vertAddr;
}

void Terrain::addSquareFace(BlockFaces face, float x, float y, float z, uint32_t color){
    /*
    v2   v3
    ______
    |   /|
    |  / |
    | /  |
    |/   |
    *-----
    v0   v1

    Triangles:
    v0 - v2 - v3
    v0 - v3 - v1
    (To preserve normal pattern)
    */

    /*
    char *facename;
    switch(face){
        case top:
            facename = "TOP";
            break;
        case bottom:
            facename = "BOTTOM";
            break;
        case left:
            facename = "LEFT";
            break;
        case right:
            facename = "RIGHT";
            break;
        case front:
            facename = "FRONT";
            break;
        case back:
            facename = "BACK";
            break;
    }

    print_arg("Adding %s face\n", (uint32_t) facename);
    */

    Vertex v0, v1, v2, v3;

    if(this->terrainMesh.numTriangles > this->maxTriangle - 2) return;
    

    //Generate Verticies
    if(face == top){
        v0.z = x;   v0.x = y;   v0.y = z+1;
        v1.z = x;   v1.x = y+1; v1.y = z+1;
        v2.z = x+1; v2.x = y;   v2.y = z+1;
        v3.z = x+1; v3.x = y+1; v3.y = z+1;
    }
    else if(face == bottom){
        v0.z = x;   v0.x = y+1; v0.y = z;
        v1.z = x;   v1.x = y;   v1.y = z;
        v2.z = x+1; v2.x = y+1; v2.y = z;
        v3.z = x+1; v3.x = y;   v3.y = z;
    }
    else if(face == left){
        v0.z = x;   v0.x = y;   v0.y = z+1;
        v1.z = x+1; v1.x = y;   v1.y = z+1;
        v2.z = x;   v2.x = y;   v2.y = z;
        v3.z = x+1; v3.x = y;   v3.y = z;
    }
    else if(face == right){
        v0.z = x+1; v0.x = y+1; v0.y = z+1;
        v1.z = x;   v1.x = y+1; v1.y = z+1;
        v2.z = x+1; v2.x = y+1; v2.y = z;
        v3.z = x;   v3.x = y+1; v3.y = z;
    }
    else if(face == front){
        v0.z = x+1; v0.x = y;   v0.y = z;
        v1.z = x+1; v1.x = y;   v1.y = z+1;
        v2.z = x+1; v2.x = y+1; v2.y = z;
        v3.z = x+1; v3.x = y+1; v3.y = z+1;
    }
    else if(face == back){
        v0.z = x;   v0.x = y;   v0.y = z+1;
        v1.z = x;   v1.x = y;   v1.y = z;
        v2.z = x;   v2.x = y+1; v2.y = z+1;
        v3.z = x;   v3.x = y+1; v3.y = z;
    }
    else{
        return;
    }

    //Find matching verticies or create new ones.
    Vertex *mesh_v0, *mesh_v1, *mesh_v2, *mesh_v3;
    mesh_v0 = this->addVertex(v0);
    mesh_v1 = this->addVertex(v1);
    mesh_v2 = this->addVertex(v2);
    mesh_v3 = this->addVertex(v3);

    if(mesh_v0 == NULL || mesh_v1 == NULL || mesh_v2 == NULL || mesh_v3 == NULL) return;


    this->terrainMesh.triangles[this->terrainMesh.numTriangles].v0 = (unsigned int) (mesh_v0 - this->terrainMesh.vertex);
    this->terrainMesh.triangles[this->terrainMesh.numTriangles].v1 = (unsigned int) (mesh_v2 - this->terrainMesh.vertex);
    this->terrainMesh.triangles[this->terrainMesh.numTriangles].v2 = (unsigned int) (mesh_v3 - this->terrainMesh.vertex);
    this->terrainMesh.triangles[this->terrainMesh.numTriangles].color = color;
    
    //print_arg("Tri%d\n", this->terrainMesh.numTriangles);
    //print_arg("V0: %d, ", this->terrainMesh.triangles[this->terrainMesh.numTriangles].v0);
    //print_arg("V1: %d, ", this->terrainMesh.triangles[this->terrainMesh.numTriangles].v1);
    //print_arg("V2: %d\n", this->terrainMesh.triangles[this->terrainMesh.numTriangles].v2);

    this->terrainMesh.triangles[this->terrainMesh.numTriangles+1].v0 = (unsigned int) (mesh_v0 - this->terrainMesh.vertex);
    this->terrainMesh.triangles[this->terrainMesh.numTriangles+1].v1 = (unsigned int) (mesh_v3 - this->terrainMesh.vertex);
    this->terrainMesh.triangles[this->terrainMesh.numTriangles+1].v2 = (unsigned int) (mesh_v1 - this->terrainMesh.vertex);
    this->terrainMesh.triangles[this->terrainMesh.numTriangles+1].color = color;

    this->terrainMesh.numTriangles+=2;
}

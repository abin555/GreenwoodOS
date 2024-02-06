#ifndef GAME_HPP
#define GAME_HPP

#include "libc.h"
#include "application.hpp"
#include "utilities.hpp"
#include "terrain.hpp"
#include "renderer.hpp"

struct IntVector3{
    int x;
    int y;
    int z;
};

class Player{
    private:
    struct Vector3 position;
    struct Vector3 rotation;
    struct Vector3 velocity;

    public:
    Player();
    Player(struct Vector3 pos, struct Vector3 rot);

    struct Vector3 *getPosition();
    void setPosition(struct Vector3 pos);
    struct Vector3 *getRotation();
    void setRotation(struct Vector3 rot);
    struct Vector3 *getVelocity();
    void setVelocity(struct Vector3 vel);
    struct Camera getCamera();

    struct IntVector3 getPlayerHeadChunkPosition();
    struct IntVector3 getPlayerFootChunkPosition();
    struct IntVector3 getLookedAtBlockCoord(Chunk *chunk);

};


#endif
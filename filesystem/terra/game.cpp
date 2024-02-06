#include "game.hpp"

Player::Player(){
    return;
}

Player::Player(struct Vector3 pos, struct Vector3 rot){
    this->position = pos;
    this->rotation = rot;
    this->velocity = {0.0f, 0.0f, 0.0f};
}

struct Vector3 *Player::getPosition(){
    return &this->position;
}

void Player::setPosition(struct Vector3 pos){
    this->position = pos;
}

struct Vector3 *Player::getRotation(){
    return &this->rotation;
}

void Player::setRotation(struct Vector3 rot){
    this->rotation = rot;
}

struct Vector3 *Player::getVelocity(){
    return &this->rotation;
}

void Player::setVelocity(struct Vector3 vel){
    this->velocity = vel;
}

struct Camera Player::getCamera(){
    struct Camera cam;
    cam.position = this->position;
    cam.rotation = this->rotation;
    return cam;
}

struct IntVector3 Player::getPlayerHeadChunkPosition(){
    struct IntVector3 vec;
    vec.y = (int)(this->position.x) + CHUNK_WIDTH/2;
    vec.x = (int)(this->position.z) + CHUNK_WIDTH/2;
    vec.z = (int)(this->position.y);
    return vec;
}

struct IntVector3 Player::getPlayerFootChunkPosition(){
    struct IntVector3 vec;
    vec.y = (int)(this->position.x) + CHUNK_WIDTH/2;
    vec.x = (int)(this->position.z) + CHUNK_WIDTH/2;
    vec.z = (int)(this->position.y - 1.4);
    return vec;
}

struct IntVector3 Player::getLookedAtBlockCoord(Chunk *chunk){
    struct IntVector3 head = this->getPlayerHeadChunkPosition();

}

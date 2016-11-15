#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "Shader.hpp"
#include "vec.h"
#include "types.h"

class Sprite;

class Entity {
public:
    
    // Constructors
    Entity();
    Entity(const Entity& rhs);
    ~Entity();
    
    // Methods
    float lerp(float v0, float v1, float t);
    void update(float elapsed);
    void render(Shader *program);
    bool collidesWith(Entity *entity);
    void checkCollisionsWithWall();
    void resolveCollision();
    void translate(float _x, float _y);
    void setUpCollBox();
    void updateCollBox(float deltaX, float deltaY);
    void moveRight(float elapsed);
    void moveLeft(float elapsed);
    void moveUp(float elapsed);
    void moveDown(float elapsed);
    void jump(float elapsed);
    void reposition(float entity, float line);
    
    // Data
    
        // Sprite Sheet
    Sprite* sprite = nullptr;
    
        // Positions
    float x;
    float y;
    float deltaX;
    float deltaY;
    
        // Dimensions
    float width;
    float height;
    
        // Physics
    float velocity_x;
    float velocity_y;
    float acceleration_x;
    float acceleration_y = -4.91;
    float friction_x = 0.8f;
    float friction_y = 0.8f;
    float totalForce;
    float mass;
    float angle;
    
        // About the Entity
    bool isStatic;
    bool isJumping;
    bool isFalling;
    EntityType entityType;
    
        // Collisions
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    collBox box;
};

#endif /* __ENTITY_HPP__ defined */

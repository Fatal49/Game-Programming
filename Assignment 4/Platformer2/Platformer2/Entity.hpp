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
    void collidesWithWall(float elapsed);
    void resolveCollision(AABB other);
    void translate(float _x, float _y);
    void setUpCollBox();
    void updateCollBox(float deltaX, float deltaY);
    void updateCollBox(vec::vec2 newPos);
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
    vec::vec2 position;
    vec::vec2 deltaPosition;
    
        // Dimensions
    float width;
    float height;
    
        // Physics
    vec::vec2 velocity;
    vec::vec2 acceleration;
    vec::vec2 gravity;
    vec::vec2 friction;
    float angle;
    
        // About the Entity
    bool isStatic = false;
    EntityType entityType;
    
        // Collisions
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    AABB box;
};

#endif /* __ENTITY_HPP__ defined */

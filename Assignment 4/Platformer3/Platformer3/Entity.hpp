#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "Shader.hpp"
#include "vec.h"
#include "types.h"
#include <math.h>

class Sprite;

class Entity {
public:
    
    // Constructors
    Entity();
    Entity(vec::vec2 p);
    Entity(const Entity& rhs);
    ~Entity();
    
    // Methods
    void setGravity(vec::vec2 g);
    void coordInit();
    void draw(Shader* shader);
    
    // Data
    
        // Sprite Sheet
        Sprite* sprite;
    
        // Positions
        vec::vec2 gridPosition;
    
        // Physics
        vec::vec2 velocity;
        vec::vec2 gravity;
    
        // About the viewport
        float openglX;
        float openglY;
        float width;
        float height;
        float tileWidth;
        float tileHeight;
    
        // About the Entity
        bool isStatic;
        EntityType entityType;
    
        // Collisions
        bool collidedTop;
        bool collidedBottom;
        bool collidedLeft;
        bool collidedRight;
    
private:
    
    // Methods
    void convertGirdToGL(float* x, float* y);
    void translate(float gridX, float gridY);
    
    // Data
    
        // coordInit() flag
        bool coordInitFlag;
    
        // Width and height of the entity
        float w;
        float h;
    
};

#endif /* __ENTITY_HPP__ defined */

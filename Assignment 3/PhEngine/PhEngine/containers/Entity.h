#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#include "SpriteSheet.h"
#include "types.h"
#include "vec.h"

class Entity {
private:
    
    // Data
    Matrix totalTransformations;
    Matrix model;
    vec::vec4 position = vec::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec::vec4 size = vec::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float deltaX = 0.0f;
    float deltaY = 0.0f;
    
    // Methods
    void addTranslation(const Matrix& m);
    void addScaling(const Matrix& m);
    void updateCollBox(vec::vec2 pos);
    
public:
    
    // Constructors
    Entity();
    Entity(unsigned int textureID, float u, float v,
           float width, float height, float size, type t, life_state ls);
    Entity(const Entity& rhs);
    
    // Data
    SpriteSheet sprite;
    float rotation;
    vec::vec2 velocity = vec::vec2(0.0f);
    type eType;
    life_state ls;
    
    // Methods
    void draw(Shader* shader);
    vec::vec4 currentPosition();
    void translateModel(float x, float y, float z);
    void scaleModel(float x, float y, float z);
    void setPosition(vec::vec2 pos);
    void detectCollision(box collBox);
    void detectCollision(Entity other);
    void update(float time);
    friend void updateDrawBox(Entity& e, box& db);
    
    // Flags
    bool collisionRight = false;
    bool collisionBottom = false;
    bool collisionLeft = false;
    bool collisionTop = false;
};

#endif /* __ENTITY_H__ defined */
#include "Entity.h"

Entity::Entity() { totalTransformations.identity(); }

Entity::Entity(unsigned int textureID, float u, float v,
                         float width, float height, float size, type t, life_state ls)
: sprite(SpriteSheet(textureID, u, v, width, height, size)), eType(t), ls(ls)
{ totalTransformations.identity(); }

Entity::Entity(const Entity& rhs) : sprite(rhs.sprite), rotation(rhs.rotation),
    position(rhs.position), velocity(rhs.velocity), size(rhs.size), model(rhs.model),
    totalTransformations(rhs.totalTransformations), eType(rhs.eType), ls(rhs.ls) {}


void Entity::addTranslation(const Matrix& m) {
    totalTransformations.m[3][0] += m.m[3][0];
    totalTransformations.m[3][1] += m.m[3][1];
    totalTransformations.m[3][2] += m.m[3][2];
}

void Entity::addScaling(const Matrix& m) {
    totalTransformations.m[0][0] += m.m[0][0];
    totalTransformations.m[1][1] += m.m[1][1];
    totalTransformations.m[2][2] += m.m[2][2];
}

void Entity::updateCollBox(vec::vec2 pos) {
    sprite.collBox.topLeft.x += pos.x;
    sprite.collBox.topLeft.y += pos.y;
    
    sprite.collBox.topRight.x += pos.x;
    sprite.collBox.topRight.y += pos.y;
    
    sprite.collBox.bottomLeft.x += pos.x;
    sprite.collBox.bottomLeft.y += pos.y;
    
    sprite.collBox.bottomRight.x += pos.x;
    sprite.collBox.bottomRight.y += pos.y;
}


void Entity::draw(Shader* shader) {
    sprite.model = totalTransformations;
    sprite.draw(shader);
}

vec::vec4 Entity::currentPosition() {
    return vec::vec4(totalTransformations.m[3][0], totalTransformations.m[3][1],
                     totalTransformations.m[3][2], totalTransformations.m[3][3]);
}

void Entity::translateModel(float x, float y, float z) {
    model.Translate(x, y, z);
    updateCollBox(vec::vec2(x, y));
    addTranslation(model);
}

void Entity::scaleModel(float x, float y, float z) {
    model.Scale(x, y, z);
    addScaling(model);
}

void Entity::setPosition(vec::vec2 destination) {
    vec::vec4 currentPos = currentPosition();
    deltaX = destination.x - currentPos.x;
    deltaY = destination.y - currentPos.y;
    translateModel(deltaX, deltaY, 0.0f);
    model.identity();
}

void Entity::detectCollision(box collBox) {
    // Top
    if (sprite.collBox.topLeft.y >= collBox.topLeft.y)
        collisionTop = true;
    else
        collisionTop = false;

    // Right
    if (sprite.collBox.topRight.x >= collBox.topRight.x)
        collisionRight = true;
    else
        collisionRight = false;
    
    // Bottom
    if (sprite.collBox.bottomLeft.y <= collBox.bottomLeft.y)
        collisionBottom = true;
    else
        collisionBottom = false;
    
    // Left
    if (sprite.collBox.bottomLeft.x <= collBox.bottomLeft.y)
        collisionLeft = true;
    else
        collisionLeft = false;
}

void Entity::detectCollision(Entity other) {
    detectCollision(other.sprite.collBox);
}

void Entity::update(float time) {
    vec::vec4 pos = currentPosition();
    vec::vec2 newPos;
    newPos.x = pos.x + (velocity.x * time) / 7.1f;
    newPos.y = pos.y + (velocity.y * time) / 4.0f;
    setPosition(newPos);
}

void updateDrawBox(Entity& e, box& db) {
    db.topLeft.x += e.deltaX;
    db.topLeft.y += e.deltaY;
    
    db.topRight.x += e.deltaX;
    db.topRight.y += e.deltaY;
    
    db.bottomLeft.x += e.deltaX;
    db.bottomLeft.y += e.deltaY;
    
    db.bottomRight.x += e.deltaX;
    db.bottomRight.y += e.deltaY;
}

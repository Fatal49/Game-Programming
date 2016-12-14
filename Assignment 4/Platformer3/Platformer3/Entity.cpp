#include "Entity.hpp"
#include "SpriteSheet.hpp"

Entity::Entity() {}

Entity::Entity(vec::vec2 p) : gridPosition(p), collidedTop(false), collidedBottom(false),
collidedLeft(false), collidedRight(false), openglX(0.0f), openglY(0.0f), width(0.0f), height(0.0f),
tileWidth(0.0f), tileHeight(0.0f), isStatic(false), sprite(nullptr), coordInitFlag(false) {}

Entity::Entity(const Entity& rhs) : gridPosition(rhs.gridPosition), gravity(rhs.gravity),
velocity(rhs.velocity), isStatic(rhs.isStatic), entityType(rhs.entityType), collidedTop(rhs.collidedTop),
collidedBottom(rhs.collidedBottom), collidedLeft(rhs.collidedLeft), collidedRight(rhs.collidedRight),
openglY(rhs.openglY), openglX(rhs.openglX), width(rhs.width), height(rhs.height),
tileWidth(rhs.tileWidth), tileHeight(rhs.tileHeight), w(rhs.w), h(rhs.h),
coordInitFlag(rhs.coordInitFlag) {
    if (sprite)
        delete sprite;
    sprite = new Sprite(*(rhs.sprite));
}

Entity::~Entity() { if (sprite) delete sprite; }

void Entity::setGravity(vec::vec2 g) { gravity = g; }

void Entity::coordInit() {
    w = openglX / (width / tileWidth);
    h = openglY / (height / tileHeight);
    
//    if (sprite) {
//        printf("coordInit: x -> %f, y -> %f\n", w * gridPosition.x, -h * gridPosition.y);
//        sprite->model.Translate(w * gridPosition.x, -h * gridPosition.y, 0.0f);
//    }
    
    coordInitFlag = true;
}

void Entity::draw(Shader* shader) { if (sprite) sprite->draw(shader); }

void Entity::convertGirdToGL(float* x, float* y) {
    if (!coordInitFlag) {
        printf("convertGridToGL: call coordInit()\n");
        return;
    }
    
    *x = w * gridPosition.x;
    *y = -h * gridPosition.y;
}

void Entity::translate(float gridX, float gridY) {
    if (sprite) {
        gridPosition.x += gridX;
        gridPosition.y += gridY;
        sprite->model.Translate(w * gridX, -h * gridY, 0.0f);
    } else {
        printf("translate: sprite not allocated\n");
    }
}

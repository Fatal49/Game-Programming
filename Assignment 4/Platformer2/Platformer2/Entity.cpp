#include "Entity.hpp"
#include "SpriteSheet.hpp"

Entity::Entity() {}

Entity::~Entity() {
    if (!sprite)
        delete sprite;
}

Entity::Entity(const Entity& rhs) : position(rhs.position), width(rhs.width), height(rhs.height),
velocity(rhs.velocity), acceleration(rhs.acceleration), isStatic(rhs.isStatic),
entityType(rhs.entityType), collidedTop(rhs.collidedTop), collidedLeft(rhs.collidedLeft),
collidedRight(rhs.collidedRight), collidedBottom(rhs.collidedBottom) {
    if (sprite)
        delete sprite;
    sprite = new Sprite(*(rhs.sprite));
}

float Entity::lerp(float v0, float v1, float t) {
    return (1.0f - t) * v0 + t * v1;
}

void Entity::update(float elapsed) {    
    if (!isStatic) {
        // Reset the flags
        collidedTop = false;
        collidedRight = false;
        collidedBottom = false;
        collidedLeft = false;
        
        // Apply gravity and velocities
        velocity += (acceleration * elapsed);
        velocity += (gravity * elapsed);
        deltaPosition = velocity * elapsed;
        
        // Update position of entity on screen and box
        translate(deltaPosition.x, deltaPosition.y);
        updateCollBox(position);
        
        printf("pos: (%f, %f)\nbox: (%f, %f)\n\n", position.x, position.y, box.center.x, box.center.y);
        
        // Check for and resolve collisions with the wall
        collidesWithWall(elapsed);
    }
}

void Entity::render(Shader* shader) {
    if (sprite)
        sprite->draw(shader);
}

bool Entity::collidesWith(Entity* entity) {
    // Detect overlap on the right or left
    if (fabsf(box.center.x - entity->box.center.x) > box.halfSize.x + entity->box.halfSize.x) {
        
        // Detect overlap on the top or bottom
        if (fabsf(box.center.y - entity->box.center.y) > box.halfSize.y + entity->box.halfSize.y) {
            return false;
        } else {
            if (box.center.y - entity->box.center.y < 0)
                collidedTop = true;
            else if (box.center.y - entity->box.center.y > 0)
                collidedBottom = true;
        }
    } else {
        if (box.center.x - entity->box.center.x < 0)
            collidedRight = true;
        else if (box.center.x - entity->box.center.x > 0)
            collidedLeft = true;
    }
    
    resolveCollision(entity->box);
    return true;
}

void Entity::collidesWithWall(float elapsed) {
    // Top
    if (box.center.y + box.halfSize.y >= 1.0f) {
        collidedTop = true;
        velocity *= -1;
        acceleration.y *= -1;
        reposition(box.center.y + box.halfSize.y, 1.0f);
    }
    
    // Right
    if (box.center.x + box.halfSize.x >= 1.0f) {
        collidedRight = true;
        velocity.x *= -1;
        acceleration.x *= -1;
        reposition(box.center.x + box.halfSize.x, 1.0f);
    }
    
    // Bottom
    if (box.center.y - box.halfSize.y <= -1.0f) {
        collidedBottom = true;
        velocity = 0.0f;
        acceleration.y = 0.0f;
        reposition(box.center.y - box.halfSize.y, -1.0f);
    }
    
    // Left
    if (box.center.x - box.halfSize.x <= -1.0f) {
        collidedLeft = true;
        velocity.x *= -1;
        acceleration.x *= -1;
        reposition(box.center.x - box.halfSize.x, -1.0f);
    }
}

void Entity::resolveCollision(AABB other) {
    if (collidedTop) {
        reposition(box.center.y + box.halfSize.y, other.center.y - other.halfSize.y);
        velocity.y = 0.0f;
    }
    
    if (collidedRight) {
        reposition(box.center.x + box.halfSize.x, other.center.x - other.halfSize.x);
        velocity.x = 0.0f;
    }
    
    if (collidedBottom) {
        reposition(box.center.y - box.halfSize.y, other.center.y + other.halfSize.y);
        velocity.y = 0.0f;
    }
    if (collidedLeft) {
        reposition(box.center.x - box.halfSize.x, other.center.x + other.halfSize.x);
        velocity.x = 0.0f;
    }
}

void Entity::translate(float _x, float _y) {
    position.x += _x;  position.y += _y;        // Update entites position
    sprite->model.Translate(_x, _y, 0.0f);      // Translate the entity
}

void Entity::setUpCollBox() {
    box.center = position;
    box.halfSize.x = width / 2;
    box.halfSize.y = height / 2;
}

void Entity::updateCollBox(float deltaX, float deltaY) {
    box.center.x += deltaX;
    box.center.y += deltaY;
}

void Entity::updateCollBox(vec::vec2 newPos) {
    box.center = newPos;
}

void Entity::moveRight(float elapsed) {
    if (!collidedRight) {
        velocity.x += 3.8f;
    }
}

void Entity::moveLeft(float elapsed) {
    if (!collidedLeft) {
        deltaPosition.x = velocity.x * elapsed * -1;
        translate(deltaPosition.x, 0.0f);
        updateCollBox(deltaPosition.x, 0.0f);
    }
}

void Entity::jump(float elapsed) {
    if (collidedBottom) {
        printf("You are jumping :)\n");
        velocity.y = 1.0f;
    }
}

void Entity::reposition(float entity, float line) {
    if (fabsf(entity) - fabsf(line) <= 0)
        return;
    
    float deltaDis = fabsf(entity) - fabsf(line);
    printf("deltaDis -> %f\n", deltaDis);
    if (entity < 0 && line < 0) {
        translate(0.0f, deltaDis);
        updateCollBox(0.0f, deltaDis);
    } else {
        translate(0.0f, deltaDis *= -1);
        updateCollBox(0.0f, deltaDis *= -1);
    }
}







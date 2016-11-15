#include "Entity.hpp"
#include "SpriteSheet.hpp"

Entity::Entity() {}

Entity::~Entity() {
    if (!sprite)
        delete sprite;
}

Entity::Entity(const Entity& rhs) : x(rhs.x), y(rhs.y), width(rhs.width), height(rhs.height),
velocity_x(rhs.velocity_x), velocity_y(rhs.velocity_y), acceleration_x(rhs.acceleration_x),
acceleration_y(rhs.acceleration_y), isStatic(rhs.isStatic), entityType(rhs.entityType),
collidedTop(rhs.collidedTop), collidedLeft(rhs.collidedLeft), collidedRight(rhs.collidedRight),
collidedBottom(rhs.collidedBottom) {
    if (sprite)
        delete sprite;
    sprite = new Sprite(*(rhs.sprite));
}

float Entity::lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

void Entity::update(float elapsed) {
    static int count = 2;
    static float appliedGravity = 0.2f;
    
    collidedTop = false;                // Reset the flags
    collidedRight = false;
    collidedBottom = false;
    collidedLeft = false;
    
    // Calculate change in y & move the entity
    if (!isJumping) {
        if (!isFalling)
            totalForce += acceleration_y * mass;      // Apply gravity
        else {
            totalForce += acceleration_y * mass * appliedGravity;
            appliedGravity += 0.2f;
            if (appliedGravity == 1.0f)
                isFalling = false;
        }
        
        deltaY = (totalForce * elapsed * elapsed) / mass;
        translate(0.0f, deltaY);
        updateCollBox(0.0f, deltaY);
        printf("x -> %f, y -> %f\n", x, y);
        printf("b.bottom -> %f\n\n", box.bottom);
        
    } else {
        
        deltaY = sinf(0.25f * M_PI * elapsed) * count;
        translate(0.0f, deltaY * 0.50f);
        updateCollBox(0.0f, deltaY * 0.50f);
        count++;
        
        if (count == 9) {
            isJumping = false;
            isFalling = true;
            count = 2;
        }
    }
    
    // Check for and resolve collisions
    checkCollisionsWithWall();
    resolveCollision();
}

void Entity::render(Shader* shader) {
    if (sprite)
        sprite->draw(shader);
}

bool Entity::collidesWith(Entity* entity) {
    
    return true;
}

void Entity::checkCollisionsWithWall() {
    if (box.top >= 1.0f) {
        collidedTop = true;
    }
    if (box.right >= 1.0f) {
        collidedRight = true;
    }
    if (box.bottom <= -1.0f) {
        printf("I am here 3\n");
        collidedBottom = true;
    }
    if (box.left <= -1.0f) {
        collidedLeft = true;
    }
}

void Entity::resolveCollision() {
    if (collidedTop)
        velocity_y *= -1;
    if (collidedRight)
        velocity_x *= -1;
    if (collidedBottom) {
        printf("I am here 1\n");
        totalForce += fabsf(acceleration_y) * mass;     // Simulate hitting the ground
        reposition(box.bottom, -1.0f);
    }
    if (collidedLeft)
        velocity_x *= -1;
}

void Entity::translate(float _x, float _y) {
    x += _x;  y += _y;                          // Update entites position
    sprite->model.Translate(_x, _y, 0.0f);      // Translate the entity
}

void Entity::setUpCollBox() {
    box.top = y + (height / 2);
    box.right = x + (width / 2);
    box.bottom = y - (height / 2);
    box.left = x - (width / 2);
}

void Entity::updateCollBox(float deltaX, float deltaY) {
    box.top += deltaY;
    box.right += deltaX;
    box.bottom += deltaY;
    box.left += deltaX;
}

void Entity::moveRight(float elapsed) {
    if (!collidedRight) {
        deltaX = velocity_x * elapsed;
        translate(deltaX, 0.0f);
        updateCollBox(deltaX, 0.0f);
    }
}

void Entity::moveLeft(float elapsed) {
    if (!collidedLeft) {
        deltaX = velocity_x * elapsed * -1;
        translate(deltaX, 0.0f);
        updateCollBox(deltaX, 0.0f);
    }
}

void Entity::moveUp(float elapsed) {
    if (!collidedTop) {
        totalForce += (mass * 18.0f);
        deltaY = (totalForce * elapsed * elapsed) / mass;
        translate(0.0f, deltaY);
        updateCollBox(0.0f, deltaY);
        reposition(box.bottom, -1.0f);
    }
}

void Entity::moveDown(float elapsed) {
    if (!collidedBottom) {
        deltaY = velocity_y * elapsed * -1;
        translate(0.0f, deltaY);
        updateCollBox(0.0f, deltaY);
    }
}

void Entity::jump(float elapsed) {
//    totalForce += (mass * acceleration_y * -2 * elapsed) / elapsed;
//    deltaY = (totalForce * elapsed * elapsed) / mass;
//    translate(0.0f, deltaY);
//    updateCollBox(0.0f, deltaY);
//    reposition(box.bottom, -1.0f);
    
//    deltaX = (acceleration_x * elapsed * elapsed);
//    deltaY = (acceleration_y * elapsed * elapsed * -50);
//    
//    translate(deltaX, deltaY);
//    updateCollBox(deltaX, deltaY);
    
//    angle += elapsed;
//    deltaX = cosf(angle);
//    deltaY = sinf(angle);
//    
//    translate(deltaX, deltaY);
//    updateCollBox(deltaX, deltaY);
    
//    acceleration_x = 3.2f;
//    
//    // Calculate the velocity
//    velocity_x = velocity_x + (acceleration_x * elapsed);
//    velocity_y = velocity_y + (acceleration_y * elapsed);
//    totalForce += mass * (velocity_x / elapsed);
    
//    deltaY = 10.0f * elapsed;
//    translate(0.0f, deltaY);
//    updateCollBox(0.0f, deltaY);
    
//    float d = 0.23f;
//    float vel = (d / elapsed) - (0.5f * fabsf(acceleration_y) * elapsed * elapsed);
//    totalForce += mass * vel;
    
//    totalForce += mass * 12.0f;
//    float d = velocity_y * elapsed - (0.5 * acceleration_y * elapsed * elapsed);
//    printf("d -> %f\n", d);
    
    isJumping = true;
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







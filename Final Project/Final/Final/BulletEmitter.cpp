#include "BulletEmitter.hpp"

BulletEmitter::BulletEmitter(unsigned int maxBullets, vec::vec2 v)
: maxBullets(maxBullets), velocity(v) {}

BulletEmitter::BulletEmitter(unsigned int maxBullets) : maxBullets(maxBullets) {}

BulletEmitter::BulletEmitter(const BulletEmitter& rhs)
: maxBullets(rhs.maxBullets), velocity(rhs.velocity), position(rhs.position) {
    // Clear the vector
    bullets.clear();
    
    // Get all of rhs's bullets
    for (size_t i = 0; i < rhs.bullets.size(); i++)
        bullets.push_back(Bullet(rhs.bullets[i].position, rhs.bullets[i].position));
}

BulletEmitter::~BulletEmitter() {}

BulletEmitter& BulletEmitter::operator=(const BulletEmitter &rhs) {
    if (this != &rhs) {
        // Get rhs's data
        velocity = rhs.velocity;
        position = rhs.position;
        maxBullets = rhs.maxBullets;
        
        // Clear the vector
        bullets.clear();
        
        // Get all of rhs's bullets
        for (size_t i = 0; i < rhs.bullets.size(); i++)
            bullets.push_back(Bullet(rhs.bullets[i].position, rhs.bullets[i].position));
    }
    
    return *this;
}

void BulletEmitter::draw() {
    
}

void BulletEmitter::update(float elapsed) {
    
}
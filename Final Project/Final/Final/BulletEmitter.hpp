#ifndef __BULLETEMITTER_HPP__
#define __BULLETEMITTER_HPP__

#include <stdio.h>
#include "Shader.hpp"
#include "vec.h"
#include <vector>

struct Bullet {
    Bullet() {}
    Bullet(vec::vec2 p, vec::vec2 v) : position(p), velocity(v) {}
    
    vec::vec2 position;
    vec::vec2 velocity;
};

class BulletEmitter {
public:
    
    // Constructors and deconstructors
    BulletEmitter(unsigned int maxBullets, vec::vec2 v);
    BulletEmitter(unsigned int maxBullets);
    BulletEmitter(const BulletEmitter& rhs);
    ~BulletEmitter();
    
    // Operator Overload
    BulletEmitter& operator = (const BulletEmitter& rhs);
    
    // Data
    vec::vec2 position;
    
    // Methods
    void setVelocity(vec::vec2 v);
    void draw();
    void update(float elapsed);
    
private:
    
    // Data
    vec::vec2 velocity;
    unsigned int maxBullets;
    std::vector<Bullet> bullets;
};


#endif /* __BULLETEMITTER_HPP__ defined */

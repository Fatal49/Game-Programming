#ifndef __PARTICLEEMITTER_HPP__
#define __PARTICLEEMITTER_HPP__

#include <stdio.h>
#include "Shader.hpp"
#include "vec.h"
#include <SDL2/SDL_opengl.h>

class ParticleEmitter {
public:
    
    // Constructors and Destructor
    ParticleEmitter(unsigned int particleCount, float maxLifeTime);
    ParticleEmitter();
    ParticleEmitter(const ParticleEmitter& rhs);
    ~ParticleEmitter();
    
    // Data
    vec::vec2 position;
    
    // Methods
    void update(float elapsed);
    void draw(Shader* shader);
    
    
private:
    
    // Data
    float maxLifeTime;
    unsigned int particleCount;
    vec::vec2 velocity;
};

#endif /* __PARTICLEEMITTER_HPP__ defined */

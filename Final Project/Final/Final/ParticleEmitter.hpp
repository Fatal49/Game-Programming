#ifndef __PARTICLEEMITTER_HPP__
#define __PARTICLEEMITTER_HPP__

#include <stdio.h>
#include "Shader.hpp"
#include "vec.h"
#include <vector>
#include <SDL2/SDL_opengl.h>

struct Particle {
    vec::vec2 position;
    vec::vec2 velocity;
    float lifetime;
};

class ParticleEmitter {
public:
    
    // Constructors and Destructor
    ParticleEmitter(unsigned int particleCount, float maxLifeTime);
    ParticleEmitter();
    ParticleEmitter(const ParticleEmitter& rhs);
    ~ParticleEmitter();
    
    // Operator overloads
    ParticleEmitter& operator = (const ParticleEmitter& rhs);
    
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
    std::vector<Particle> particles;
};

#endif /* __PARTICLEEMITTER_HPP__ defined */

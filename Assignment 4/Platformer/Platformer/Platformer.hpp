#ifndef __PLATFORMER_HPP__
#define __PLATFORMER_HPP__

#include "Game.hpp"
#include "Entity.hpp"
#include "SpriteSheet.hpp"
#include "vec.h"
#include <SDL2_image/SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Platformer : public Game {
public:
    
    // Constructors and Destructors
    Platformer(float w, float h, const char* name);
    Platformer(const char* name);
    ~Platformer();
    
    // Overload functions
    void setup();
    bool processEvents();
    void update();
    void render();
    
    // Methods
    void draw();
    
private:
    
    // Methods
    void init();
    GLuint LoadTexture(const char *image_path);
    
    // For animations
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float elapsed = 0.0f;
    
    // Data
    
        // Shader
    Shader* shader = nullptr;
    
        // Matrices
    Matrix model, view, projection;
    
        // ID's
    GLuint spriteSheetID;
    
        // SpriteSheet
    SpriteSheet* ss = nullptr;
    
        // Entities
    std::vector<Entity> entities;
    
};


#endif /* __PLATFORMER_HPP__ defined */

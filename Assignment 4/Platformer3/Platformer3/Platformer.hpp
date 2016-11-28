#ifndef __PLATFORMER_HPP__
#define __PLATFORMER_HPP__

#include "Game.hpp"
#include "vec.h"
#include <vector>
#include "Tile.hpp"
#include <SDL2_image/SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define LEVEL_WIDTH 24
#define LEVEL_HEIGHT 16
#define TILE_WIDTH 16.0f
#define TILE_HEIGHT 16.0f
#define SPRITE_SHEET_WIDTH 384.0f
#define SPRITE_SHEET_HEIGHT 256.0f
#define DRAW_SIZE 0.025f
#define SPRITE_COUNT_X 24.0f
#define SPRITE_COUNT_Y 16.0f

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
    void setStaticEntities();
    
    // For animations
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float elapsed = 0.0f;
    
    // Data
    
        // Shader
        Shader* shader = nullptr;
    
        // Tile
        Tile* tile = nullptr;
    
        // Matrices
        Matrix model, view, projection;
    
        // Texture ID's
        GLuint tex1;        
    
        // Containers        
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> texCoords;
};


#endif /* __PLATFORMER_HPP__ defined */

#ifndef __PLATFORMER_HPP__
#define __PLATFORMER_HPP__

#include "Game.hpp"
#include "vec.h"
#include "Shader.hpp"
#include "Matrix.hpp"
#include "Rectangle.hpp"
#include <string>
#include <SDL2_image/SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

enum gameState { START_SCREEN, GAME };

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
    void DrawText(Shader *program, GLuint fontTexture,
                  std::string text, float size, float spacing, bool fade);

    // For animations
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float elapsed = 0.0f;
    
    // Flags
    bool pause = false;
    
    // Game States
    gameState gs;
    
    // Data
    
        // Shader
        Shader* shader = nullptr;
        Shader* texShader = nullptr;
    
        // Matrices
        Matrix model, view, projection, matrix;
    
        // Texture ID's
        GLuint fontTexture;
    
        // Players
        Rectangle* player1 = nullptr;
        Rectangle* player2 = nullptr;
};

#endif /* __PLATFORMER_HPP__ defined */

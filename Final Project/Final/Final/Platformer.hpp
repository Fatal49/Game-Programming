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

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

// Angle that controls fading
#define FADE 90.0f

enum gameState { START_SCREEN, GAME };
enum fade { FADING_IN, FADING_OUT };

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
    float easeInOut(float from, float to, float time);
    float lerp(float from, float to, float time);
    
    // For animations
    float lastFrameTicks = 0.0f;
    float totalTime = 0.0f;
    float elapsed = 0.0f;
    
    // Flags
    bool pause = false;
    
    // Enums
    gameState gs;
    fade fading;
    
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

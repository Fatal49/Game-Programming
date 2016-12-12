#ifndef __PLATFORMER_HPP__
#define __PLATFORMER_HPP__

#include "Game.hpp"
#include "vec.h"
#include "Shader.hpp"
#include "Matrix.hpp"
#include "Rectangle.hpp"
#include "BulletEmitter.hpp"
#include <string>
#include <SDL2_image/SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

// Angle that controls fading
#define FADE 100.0f

// Enumerations
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
    float lastFrameTicks;
    float totalTime;
    float elapsed;
    
    // Flags
    bool pause;
    
    // Enums
    gameState gs;
    fade fading;
    
    // Background Colors
    vec::vec4 lightBlack = vec::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    vec::vec4 black = vec::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Data
    
        // Shader
        Shader* shader;
        Shader* texShader;
    
        // Matrices
        Matrix model, view, projection, matrix;
    
        // Texture ID's
        GLuint fontTexture;
    
        // Players
        Rectangle* player1;
        Rectangle* player2;
};

#endif /* __PLATFORMER_HPP__ defined */

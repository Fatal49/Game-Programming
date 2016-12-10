#ifndef __PLATFORMER_HPP__
#define __PLATFORMER_HPP__

#include "Game.hpp"
#include "Matrix.hpp"
#include "Rectangle.hpp"
#include <time.h>
#include <SDL2_image/SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define OPENGL_X 7.1f
#define OPENFL_Y 4.0f

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
    void checkCollisions();
    
    // Methods
    void draw();
    
private:
    
    // Methods
    void init();
    bool testSATSeparationForEdge(float edgeX, float edgeY,
                                  const std::vector<vec::vec2>& points1,
                                  const std::vector<vec::vec2>& points2);
    bool checkSATCollision(const std::vector<vec::vec2> &e1Points,
                           const std::vector<vec::vec2> &e2Points);
    void checkCollisionWithWall(Rectangle* rec, float opengl_x, float opengl_y);

    // For animations
    float lastFrameTicks = 0.0f;
    float elapsed = 0.0f;
    
    // Flags
    bool pause = false;    
    
    // Data
    
        // Shader
        Shader* shader = nullptr;
    
        // Matrices
        Matrix model, view, projection;
    
        // Texture ID's
        GLuint tex1;
    
        // Objects
        Rectangle* rec1 = nullptr;
        Rectangle* rec2 = nullptr;
        Rectangle* rec3 = nullptr;    
};

#endif /* __PLATFORMER_HPP__ defined */

#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2_image/SDL_image.h>
#include "sdl/sdl_app.h"
#include "shaders/Shader.h"
#include "containers/Matrix.h"
#include "containers/SpriteSheet.h"
#include "containers/Entity.h"
#include "containers/types.h"
#include <vector>
#include <cmath>

#define SHADERS "shaders/"
#define IMAGE "images/"
#define MAX_SPRITES_ROW 4
#define MAX_SPRITES_COL 8
#define X_RES 1280
#define Y_RES 800
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BULLETS 10

// Game states
enum game_state { TITLE_SCREEN, GAME };

class Game : public App {
private:
    
    // Game states
    game_state gs = TITLE_SCREEN;
    
    // Containers
    std::vector<Entity> invaders;
    std::vector<std::vector<Entity>> invaderGrid;
    std::vector<Entity> bullets;
    Entity destruction;
    Entity player;
    
    // Data
    float bulletHalfHeight = 0.0f;
    GLuint fontTexture;
    
    // Shaders
    Shader* shader = nullptr;
    
    // Matrices
    Matrix model, view, projection;
    
    // Width & height
    int width, height;
    
    // Invaders box
    box db;
    box window;
    
    // Methods
    void init();
    void setupScene();  
    
public:
    
    // Constructor & Destructor
    Game(Uint32 flags, const char* name = "demo", int w = 1280, int h = 800);
    ~Game();
    
    // Methods
    GLuint LoadTexture(const char *image_path);
    void update(float elapsed, eventParse ep);
    void onDraw(const eventParse& ep, float elapsed, float angle);
    void DrawText(Shader *program, GLuint fontTexture,
                  std::string text, float size, float spacing);
    
};

#endif /* __GAME_H__ defined */

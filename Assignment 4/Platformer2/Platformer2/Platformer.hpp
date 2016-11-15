#ifndef __PLATFORMER_HPP__
#define __PLATFORMER_HPP__

#include "Game.hpp"
#include "Entity.hpp"
#include "SpriteSheet.hpp"
#include "vec.h"
#include "Tile.hpp"
#include <SDL2_image/SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define LEVEL_WIDTH 24
#define LEVEL_HEIGHT 16

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
    int findEntity(int n) const;
    void setStaticEntities();
    // void drawMap();
    
    // For animations
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float elapsed = 0.0f;
    
    // Data
    
    int DRAW_E = 0;
    
        // Shader
    Shader* shader = nullptr;
    
        // Matrices
    Matrix model, view, projection;
    
        // Tile
    Tiles* t = nullptr;
    GLuint textureID;
    
        // TileSheet
    TileSheet* ts = nullptr;
    
    // Tile info
        float TILE_WIDTH;
        float TILE_HEIGHT;
        float SPRITE_COUNT_X;
        float SPRITE_COUNT_Y;
    
        // Entities
    std::vector<tile> tiles;
    std::vector<entityTile> entities;
    std::vector<std::vector<unsigned char>> map;
    unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
    
        // Map info
    std::vector<unsigned short> staticEntities;
};


#endif /* __PLATFORMER_HPP__ defined */

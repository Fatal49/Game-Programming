#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include <SDL2_image/SDL_image.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Entity.hpp"
#include "types.h"
#include "Tile.hpp"

#define DRAW_NO_TILE 1000

class Sprite {
private:
    
    // Methods
    void init();
    
public:
    
    // Constructors
    Sprite();
    Sprite(GLuint textureID, float u, float v,
                float width, float height, float size);
    Sprite(const Sprite& rhs);
    
    // Data
    float size;
    GLuint textureID;
    float u;
    float v;
    float width;
    float height;
    float texHeight;
    float texWidth;
    Matrix model, view, projection;
    GLfloat texCoords[12];
    float vertices[12];
    
    // Methods
    void draw(Shader *program);
};

#endif /* __SPRITESHEET_H__ defined */

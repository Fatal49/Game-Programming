#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include <SDL2_image/SDL_image.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Shader.hpp"
#include "types.h"

class Sprite {
private:
    
    // Methods
    void init();
    
public:
    
    // Constructors
    Sprite();
    Sprite(GLuint textureID, float u, float v,
           float width, float height, float tw, float th, float openGlX,
           float openglY, float w, float h, float s);
    Sprite(const Sprite& rhs);
    
    // Data
    GLuint textureID;
    float u;
    float v;
    float width;
    float height;
    float tw;
    float th;
    float openGlX;
    float openGlY;
    float w;
    float h;
    float size;
    Matrix model, view, projection;
    GLfloat texCoords[12];
    float vertices[12];
    
    // Methods
    void draw(Shader *program);
};

#endif /* __SPRITESHEET_H__ defined */

#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include "../shaders/Shader.h"
#include "types.h"
#include <vector>

class SpriteSheet {
private:
    
    void init();
    
public:
    
    // Constructors
    SpriteSheet();
    SpriteSheet(unsigned int textureID, float u, float v,
                float width, float height, float size);
    SpriteSheet(const SpriteSheet& rhs);
    
    // Data
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
    float texHeight;
    float texWidth;
    Matrix model, view, projection;
    GLfloat texCoords[12];
    float vertices[12];
    box collBox;
    
    // Methods
    void draw(Shader *program);
};

#endif /* __SPRITESHEET_H__ defined */

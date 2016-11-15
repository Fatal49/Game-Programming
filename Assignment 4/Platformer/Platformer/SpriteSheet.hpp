#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Entity.hpp"
#include "types.h"

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


class SpriteSheet {
public:
    
    // Constructors
    SpriteSheet(const char* fn, float w, float h);
    SpriteSheet(const SpriteSheet& rhs);
    
    // Methods
    void extractSpriteInfo();
    void fillSprites(std::vector<Sprite>& vec, GLuint textureID, float size);
    void fillEntities(std::vector<Entity>& obj, GLuint textureID, float size);
    
private:
    
    // Data
        // Filename
        const char* fileName;
    
        // Sprite sheet dimensions
        float width, height;
    
        // Container
        std::vector<spriteInfo> info;
};

#endif /* __SPRITESHEET_H__ defined */

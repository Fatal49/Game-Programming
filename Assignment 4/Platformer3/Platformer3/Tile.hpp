 
#ifndef __TILE_HPP__
#define __TILE_HPP__

#include "types.h"
#include "Shader.hpp"
#include "Matrix.hpp"
#include "Entity.hpp"
#include <fstream>
#include <vector>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>


class Tile {
public:
    
    // Constructors
    Tile(float w, float h, float tw, float th, float x_units, float y_units);
    Tile(const Tile& rhs);
    
    // Data
    Matrix model;
    Matrix view;
    Matrix projection;
    
    // Methods
    void readFlareMap(const char* fn, std::vector<Entity*>& entities);
    void useMapData(int SPRITE_COUNT_X, int SPRITE_COUNT_Y, float TILE_SIZE);
    void draw(Shader* program);
    void loadTexture(const char* image_path);
    GLuint getTextureId() const;
    
private:
    
    // Sprite sheet info
    float TILE_WIDTH;
    float TILE_HEIGHT;
    
    // Game info
    float SCREEN_WIDTH_PIXELS;
    float SCREEN_HEIGHT_PIXELS;
    float SCREEN_WIDTH_OPENGL;
    float SCREEN_HEIGHT_OPENGL;
    float LEVEL_WIDTH;
    float LEVEL_HEIGHT;
    
    // Data
    GLuint textureID;
    std::vector<std::vector<unsigned short>> levelData;
    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> texCoords;
    
    // Methods
    void getHeaderInfo(std::ifstream& ipf);
    void readMapData(std::ifstream& ipf);
    void getEntityData(std::ifstream& ipf, std::vector<Entity*>& entities);
    
};

#endif /* __TILE_HPP__ defined */

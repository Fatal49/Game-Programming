#ifndef __TILE_HPP__
#define __TILE_HPP__

#include "types.h"
#include "vec.h"
#include "Matrix.hpp"
#include <SDL2_image/SDL_image.h>
#include <vector>
#include <string>
#include <fstream>
#include "Entity.hpp"

struct entityTile {
    entityTile() {}
    entityTile(vec::vec2 p, unsigned short n) : position(p), num(n) {}
    entityTile(const entityTile& rhs) : position(rhs.position), num(rhs.num) {}
    entityTile& operator = (const entityTile& rhs) {
        if (this != &rhs) {
            position = rhs.position;
            num = rhs.num;
        }
        
        return *this;
    }
    
    vec::vec2 position;
    unsigned short num;
};

class Tiles {
public:
    
    // Constructors
    Tiles(float ts, int sc_x, int sc_y, int lw, int lh, GLuint tID);
    Tiles(const Tiles& rhs);
    
    // Data
    Matrix model;
    Matrix view;
    Matrix projection;
    
    // Methods
    void init(std::vector<std::vector<unsigned char>>& levelData);
    void readInLevel(const char* fn,
                     std::vector<std::vector<unsigned char>>& levelData,
                     std::vector<entityTile>& entities);
    void draw(Shader* program);

private:
    
    // Data
    
        // File name
        const char* fn;
    
        // Texture ID
        GLuint textureID;
    
        // Tile info
        float TILE_SIZE;
        int SPRITE_COUNT_X;
        int SPRITE_COUNT_Y;
        int LEVEL_HEIGHT;
        int LEVEL_WIDTH;
    
        // Containers
        std::vector<float> vertexData;
        std::vector<float> texCoordData;
    
        // Methods
    void readMapData(std::ifstream& ipf,
                     std::vector<std::vector<unsigned char>>& grid);
    void readEntitiesData(std::ifstream& ipf, std::vector<entityTile>& entities);
};


#endif /* __TILE_HPP__ defined */

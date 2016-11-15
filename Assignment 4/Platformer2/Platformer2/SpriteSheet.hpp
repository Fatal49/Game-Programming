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

// Tile info
struct tile {
    tile() {}
    tile(const Sprite& tile, const Entity& entity, unsigned short n)
    : s(tile), e(entity), num(n) {}
    tile(const tile& rhs) : num(rhs.num), s(Sprite(rhs.s)), e(Entity(rhs.e)) {}
    
    Sprite s;
    Entity e;
    unsigned short num;
};

class TileSheet {
public:

    // Constructors
    TileSheet(const char* _fn, float i_w, float i_h, float t_w, float t_h,
              float r, float c, float _s);
    TileSheet(const TileSheet& rhs);
    
    // Methods
    void loadTiles(std::vector<tile>& tiles);
    void getTileIndex(const std::string& fn, std::vector<tile>& tiles);
    void buildMap(const std::string& fn, std::vector<std::vector<unsigned short>>& map,
                  std::vector<tile>& tiles, std::vector<entityTile>& entities);
    
private:
    // Data
        // Filename
        const char* fn;
    
        // Tile sheet ID
        GLuint tileSheetID;
    
        // Tile info
        float image_h;
        float image_w;
        float tile_w;
        float tile_h;
        float rows;
        float columns;
        float size;
    
        // Methods
        void readMapData(std::ifstream& ipf,
                         std::vector<std::vector<unsigned short>>& grid);
        void readEntitiesData(std::ifstream& ipf, std::vector<entityTile>& entities);
        GLuint LoadTexture(const char* image_path);
};

#endif /* __SPRITESHEET_H__ defined */

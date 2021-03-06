#include "Tile.hpp"

Tile::Tile(float w, float h, float tw, float th, float x_units, float y_units)
: SCREEN_WIDTH_PIXELS(w), SCREEN_HEIGHT_PIXELS(h), TILE_WIDTH(tw), TILE_HEIGHT(th),
SCREEN_WIDTH_OPENGL(x_units), SCREEN_HEIGHT_OPENGL(y_units) {}

void Tile::readFlareMap(const char* fn, std::vector<Entity*>& entities) {
    // Open stream
    std::ifstream ipf(fn);
    
    // Error check
    if (!ipf) {
        printf("readFlareMap: Could not open %s\n", fn);
        return;
    }
    
    std::string line;
    while (getline(ipf, line)) {
        
        if (line == "[header]") {
            getHeaderInfo(ipf);
        }
        
        if (line == "data=") {
            readMapData(ipf);
        }
        
        if (line == "[Enemies]" || line == "[Players]") {
            getEntityData(ipf, entities);
        }
    }
    
    // Close stream
    ipf.close();
}

void Tile::useMapData(int SPRITE_COUNT_X, int SPRITE_COUNT_Y, float TILE_SIZE) {
    float TS_WIDTH = (SCREEN_WIDTH_OPENGL / (SCREEN_WIDTH_PIXELS / TILE_WIDTH));
    float TS_HEIGHT = (SCREEN_HEIGHT_OPENGL / (SCREEN_HEIGHT_PIXELS / TILE_HEIGHT));
    
    for(int y = 0; y < LEVEL_HEIGHT; y++) {
        for(int x = 0; x < LEVEL_WIDTH; x++) {
            
            if (levelData[y][x] != 0) {
//                float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X);
//                float v = (int)(((int)levelData[y][x]) / SPRITE_COUNT_X);
                
                float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
                float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
                
                // Triangle vertices
//                vertexData.insert(vertexData.end(), {
//                    x * TS_WIDTH, y * -TS_HEIGHT,                               // Top Left
//                    x * TS_WIDTH, (y * -TS_HEIGHT) - TS_HEIGHT,                 // Bottom Left
//                    (x * TS_WIDTH) + TS_WIDTH, y * -TS_HEIGHT,                  // Top Right
//                    x * TS_WIDTH, (y * -TS_HEIGHT) - TS_HEIGHT,                 // Bottom Left
//                    (x * TS_WIDTH) + TS_WIDTH, (y * -TS_HEIGHT) - TS_HEIGHT,    // Bottom Right
//                    (x * TS_WIDTH) + TS_WIDTH, y * -TS_HEIGHT,                  // Top Right
//                });
//                
//                // Texture coordinates
//                texCoords.insert(texCoords.end(), {
//                    u * spriteWidth, v * spriteHeight,                                  // Top Left
//                    u * spriteWidth, (v * spriteHeight) + spriteHeight,               // Bottom Left
//                    (u * spriteWidth) + spriteWidth, v * spriteHeight,                  // Top Right
//                    u * spriteWidth, (v * spriteHeight) + spriteHeight,                 // Bottom Left
//                    (u * spriteWidth) + spriteWidth, (v * spriteHeight) + spriteHeight, // Bottom Right
//                    (u * spriteWidth) + spriteWidth, v * spriteHeight,                   // Top Right
//                });
                
                float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;

                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    TILE_SIZE * x, -TILE_SIZE * y,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
                });
                
                texCoords.insert(texCoords.end(), {
                    u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u, v,
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v
                });
            }
        }
    }
}

void Tile::draw(Shader* program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    program->setModelMatrix(model);
    program->setViewMatrix(view);
    program->setProjectionMatrix(projection);
    
    glVertexAttribPointer(program->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertexData[0]);
    glEnableVertexAttribArray(program->getPositionAttrib());
    glVertexAttribPointer(program->getTexCoordAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
    glEnableVertexAttribArray(program->getTexCoordAttrib());
    
    glDrawArrays(GL_TRIANGLES, 0, (int)vertexData.size());
    
    glDisableVertexAttribArray(program->getPositionAttrib());
    glDisableVertexAttribArray(program->getTexCoordAttrib());
}

void Tile::loadTexture(const char* image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tile::getHeaderInfo(std::ifstream& ipf) {
    std::string info, temp;
    size_t start;
    
    // Get the LEVEL_WIDTH
    getline(ipf, info);
    start = info.find("=", 0);
    temp = info.substr(start + 1, info.size() - start);
    LEVEL_WIDTH = (float)atoi(temp.c_str());
    
    // Get the LEVEL_HEIGHT
    getline(ipf, info);
    start = info.find("=", 0);
    temp = info.substr(start + 1, info.size() - start);
    LEVEL_HEIGHT = (float)atoi(temp.c_str());
}

void Tile::readMapData(std::ifstream& ipf) {
    std::string line, index;
    
    // Read the data until the end
    while (getline(ipf, line)) {
        if (line == "") break;
        
        std::istringstream iss(line);
        std::vector<unsigned short> row;
        
        while (getline(iss, index, ',')) {
            unsigned short x = (unsigned short)atoi(index.c_str());
            if (x > 0)
                row.push_back(x - 1);           // tile index
            else
                row.push_back(0);               // # to indicate no tile
        }
        levelData.push_back(row);
    }
}

void Tile::getEntityData(std::ifstream& ipf, std::vector<Entity*>& entities) {
    std::string line, temp;
    size_t start;
    EntityType et;
    int x = 0, y = 0;
    
    // Get to the important stuff
    getline(ipf, line);
    getline(ipf, line);
    
    // Get the entities type
    start = line.find("=", 0);
    temp = line.substr(start + 1, line.size() - start);
    
    if (temp == "ENTITY_ENEMY")
        et = ENTITY_ENEMY;
    if (temp == "ENTITY_PLAYER")
        et = ENTITY_PLAYER;
    if (temp == "ENTITY_COIN")
        et = ENTITY_COIN;
    
    // Get x location
    getline(ipf, line);
    start = line.find("=", 0);
    temp = line.substr(start + 1, line.find(",", start) - start - 1);
    x = (int)atoi(temp.c_str());
    
    // Get y location
    start = line.find(",", start);
    temp = line.substr(start + 1, line.find(",", start) - start - 1);
    y = (int)atoi(temp.c_str()) - 1;
    
    entities.push_back(new Entity(vec::vec2(x, y)));
}

GLuint Tile::getTextureId() const { return textureID; }











#include "Tile.hpp"

Tiles::Tiles(float ts, int sc_x, int sc_y, int lw, int lh, GLuint tID)
: TILE_SIZE(ts), SPRITE_COUNT_X(sc_x), SPRITE_COUNT_Y(sc_y), LEVEL_WIDTH(lw), LEVEL_HEIGHT(lh),
textureID(tID) {}

void Tiles::setMap(std::vector<std::vector<unsigned char>>& levelData) {
    for(int y = 0; y < LEVEL_HEIGHT; y++) {
        for(int x = 0; x < LEVEL_WIDTH; x++) {
            
            if (levelData[y][x] != 0) {
                float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
                
                float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
                float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
                
                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    TILE_SIZE * x, -TILE_SIZE * y,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
                });
                
                texCoordData.insert(texCoordData.end(), {
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

void Tiles::readInLevel(const char* fn,
                        std::vector<std::vector<unsigned char>>& levelData,
                        std::vector<Entity*>& entities) {
    view.Translate(-1.0f, 2.1f, 0.0f);
    
    // Open the file
    std::ifstream ipf(fn);
    
    // Error check
    if (!ipf) {
        printf("buildMap: Cannot open the spcified file.\n");
        return;
    }
    
    std::string line;
    while(getline(ipf, line)) {
        // Read the layer portion
        if (line == "[layer]") {
            getline(ipf, line);
            
            // Get to the 'data=' line
            if (line != "data=") {
                while (getline(ipf, line)) {
                    if (line == "data=") {
                        break;
                    }
                }
            }
            
            // Get the map data
            readMapData(ipf, levelData);
        }
        
        // Read in players
        if (line == "[Player]" || line == "[Enemies]" || line == "[Coins]")
            readEntitiesData(ipf, entities);
    }
    
    // Close the file
    ipf.close();
}

void Tiles::readMapData(std::ifstream& ipf,
                            std::vector<std::vector<unsigned char>>& grid) {
    std::string line, index;
    
    // Read the data until the end
    while (getline(ipf, line)) {
        if (line == "") break;
        
        std::istringstream iss(line);
        std::vector<unsigned char> row;
        
        while (getline(iss, index, ',')) {
            unsigned char x = (unsigned char)atoi(index.c_str());
            if (x > 0)
                row.push_back(x - 1);           // tile index
            else
                row.push_back(0);               // # to indicate no tile
        }
        
        grid.push_back(row);
        row.clear();
    }
}

void Tiles::readEntitiesData(std::ifstream& ipf, std::vector<Entity*>& entities) {
    std::string line;
    int x = 0, y = 0;
    
    // Get to the entities type
    while (getline(ipf, line)) {
        if (line.size() > 8) {
            if (line.substr(0, 4) == "type")
                break;
        }
    }
    
    // Get entity type and go to the next line
    std::string type = line.substr(5, line.size() - 5);
    getline(ipf, line);
    
    // Get x position
    size_t start = line.find("=", 0);
    std::string temp = line.substr(start + 1, line.find(",", start) - start - 1);
    x = atoi(temp.c_str());
    
    // Get y position
    start = line.find(",", start);
    temp = line.substr(start + 1, line.find(",", start) - start - 1);
    y = atoi(temp.c_str()) - 1.0f;
    
    // Create and place the entity
    placeEntity(type, (float)x, (float)y, entities);
    
}

void Tiles::placeEntity(const std::string& type, float placeX, float placeY,
                        std::vector<Entity*>& entities) {
    EntityType et;
    Entity* e;
    
    // Get type
    if (type == "ENTITY_PLAYER")
        et = ENTITY_PLAYER;
    else if (type == "ENTITY_ENEMY")
        et = ENTITY_ENEMY;
    else if (type == "ENTITY_COIN")
        et = ENTITY_COIN;
    
    // Create entity
    e = new Entity();
    e->gridPos = vec::vec2(placeX, placeY);
    e->position = vec::vec2(placeX * TILE_SIZE, placeY * -TILE_SIZE);
    e->view = view;
    e->entityType = et;
    
    // Place entity in container
    entities.push_back(e);
}

void Tiles::draw(Shader* program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    program->setModelMatrix(model);
    program->setViewMatrix(view);
    program->setProjectionMatrix(projection);
    
    glVertexAttribPointer(program->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertexData[0]);
    glEnableVertexAttribArray(program->getPositionAttrib());
    glVertexAttribPointer(program->getTexCoordAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &texCoordData[0]);
    glEnableVertexAttribArray(program->getTexCoordAttrib());
    
    glDrawArrays(GL_TRIANGLES, 0, (int)vertexData.size());
    
    glDisableVertexAttribArray(program->getPositionAttrib());
    glDisableVertexAttribArray(program->getTexCoordAttrib());
}



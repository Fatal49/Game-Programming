#include "SpriteSheet.hpp"

Sprite::Sprite() {}

Sprite::Sprite(GLuint textureID, float u, float v,
                         float width, float height, float size)
: textureID(textureID), u(u), v(v), width(width), height(height), size(size) { init(); }

Sprite::Sprite(const Sprite& rhs)
: textureID(rhs.textureID), u(rhs.u), v(rhs.v), width(rhs.width), height(rhs.height), size(rhs.size),
model(rhs.model), view(rhs.view), projection(rhs.projection), texWidth(rhs.texWidth),
texHeight(rhs.texHeight) {
    try {
        for (int i = 0; i < 12; i++) {
            vertices[i] = rhs.vertices[i];
            texCoords[i] = rhs.texCoords[i];
        }
    } catch (...) {
        printf("There was a problem\n");
    }
    
}

void Sprite::init() {
    // Sprite texture coordinates
    texCoords[0] = u;
    texCoords[1] = v + height;
    texCoords[2] = u + width;
    texCoords[3] = v;
    texCoords[4] = u;
    texCoords[5] = v;
    texCoords[6] = u + width;
    texCoords[7] = v;
    texCoords[8] = u;
    texCoords[9] = v + height;
    texCoords[10] = u + width;
    texCoords[11] = v + height;
    
    // Aspect ratio
    float aspect = width / height;
    
    // Sprite vertices
    vertices[0] = -0.5 * size * aspect;
    vertices[1] = -0.5 * size;
    vertices[2] = 0.5 * size * aspect;
    vertices[3] = 0.5 * size;
    vertices[4] = -0.5 * size * aspect;
    vertices[5] = 0.5 * size;
    vertices[6] = 0.5 * size * aspect;
    vertices[7] = 0.5f * size;
    vertices[8] = -0.5f * size * aspect;
    vertices[9] = -0.5f * size;
    vertices[10] = 0.5f * size * aspect;
    vertices[11] = -0.5f * size;

    texWidth = fabsf(vertices[0]) * 2;
    texHeight = fabsf(vertices[1]) * 2;
}

void Sprite::draw(Shader *program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set matrices
    program->setModelMatrix(model);
    program->setViewMatrix(view);
    program->setProjectionMatrix(projection);
    
    // Draw sprite
    glVertexAttribPointer(program->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glEnableVertexAttribArray(program->getPositionAttrib());
    glVertexAttribPointer(program->getTexCoordAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
    glEnableVertexAttribArray(program->getTexCoordAttrib());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->getPositionAttrib());
    glDisableVertexAttribArray(program->getTexCoordAttrib());
}


TileSheet::TileSheet(const char* _fn, float i_w, float i_h, float t_w, float t_h,
                     float r, float c,  float _s)
: fn(_fn), image_w(i_w), image_h(i_h), tile_w(t_w), tile_h(t_h), size(_s), rows(r), columns(c)
{ tileSheetID = LoadTexture(_fn); }

TileSheet::TileSheet(const TileSheet& rhs) : fn(rhs.fn), image_w(rhs.image_w), image_h(rhs.image_h), tile_w(rhs.tile_w), tile_h(rhs.tile_h), size(rhs.size), tileSheetID(rhs.tileSheetID),
    rows(rhs.rows), columns(rhs.columns) {}

void TileSheet::loadTiles(std::vector<tile>& tiles) {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            tile t;
            
            t.s = Sprite(tileSheetID, (x * tile_w)/image_w, (y * tile_h)/ image_h,
                         tile_w/image_w, tile_h/image_h, size);
            
            Entity temp;
            temp.width = t.s.texWidth;
            temp.height = t.s.texHeight;
            temp.sprite = &t.s;
            t.e = temp;
            
            tiles.push_back(t);
        }
    }
}

void TileSheet::getTileIndex(const std::string& fn, std::vector<tile>& tiles) {
    if (fn.size() < 4) {
        printf("getFileTags: Please use a file name with an extension.\n");
        return;
    }
    
    if (fn.substr(fn.size() - 3, 3) != "csv") {
        printf("getFileTags: Please a file with the extension .csv\n");
        return;
    }
    
    // Open the file
    std::ifstream ipf(fn);
    std::string line;
    int count = 0;
    
    // Error check
    if (!ipf) {
        printf("getFileTags: Cannot open the specified file.\n");
        return;
    }
    
    // Read the file
    while (getline(ipf, line)) {
        std::string index;
        std::istringstream lineStream(line);
        
        for (int i = 0; i < columns; i++) {
            count++;
            getline(lineStream, index, ',');
            tiles[count].num = (unsigned short)atoi(index.c_str());
        }
    }
    
    // Close the file
    ipf.close();
}

void TileSheet::buildMap(const std::string& fn, std::vector<std::vector<unsigned short>>& map,
                         std::vector<tile>& tiles, std::vector<entityTile>& entities) {
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
            readMapData(ipf, map);
        }
        
        // Read in players
        if (line == "[Player]" || line == "[Enemies]" || line == "[Coins]")
            readEntitiesData(ipf, entities);
    }
    
    // Close the file
    ipf.close();
}

void TileSheet::readMapData(std::ifstream& ipf,
                            std::vector<std::vector<unsigned short>>& grid) {
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
                row.push_back(DRAW_NO_TILE);    // # to indicate no tile
        }
        
        grid.push_back(row);
        row.clear();
    }
}

void TileSheet::readEntitiesData(std::ifstream& ipf, std::vector<entityTile>& entities) {
    std::string line;
    unsigned short x = 0, y = 0, num = 0;
    
    // Get to the entities location
    while (getline(ipf, line)) {
        if (line.size() > 8) {
            if (line.substr(0, 8) == "location")
                break;
        }
    }
    
    // Get x position
    size_t start = line.find("=", 0);
    std::string temp = line.substr(start + 1, line.find(",", start) - start - 1);
    x = (unsigned short)atoi(temp.c_str());
    
    // Get y position
    start = line.find(",", start);
    temp = line.substr(start + 1, line.find(",", start) - start - 1);
    y = (unsigned short)atoi(temp.c_str());
    
    // Get index
    getline(ipf, line);
    start = line.find("=", 0);
    temp = line.substr(start + 1, line.size() - start);
    num = (unsigned short)atoi(temp.c_str());
    
    // Add the data to entities
    entities.push_back(entityTile(vec::vec2(x, y), num));
}

GLuint TileSheet::LoadTexture(const char* image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}



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


SpriteSheet::SpriteSheet(const char* fn, float w, float h)
: fileName(fn), width(w), height(h) {}

SpriteSheet::SpriteSheet(const SpriteSheet& rhs)
: fileName(rhs.fileName), width(rhs.width), height(rhs.height) {
    info.clear();
    for (size_t i = 0; i < rhs.info.size(); i++)
        info.push_back(rhs.info[i]);
}

void SpriteSheet::extractSpriteInfo() {
    // Create stream
    std::fstream ipf(fileName);
    
    // Error check
    if (!ipf) {
        printf("There was an error opening the file\n");
        return;
    }
    
    // Clear flags and set the pointer to the beg. of the file
    ipf.clear();
    ipf.seekg(0);
    
    std::string content;
    size_t space;
    spriteInfo si;
    std::getline(ipf, content);                     // Read the first line
    
    // Read contents of the file
    while (std::getline(ipf, content)) {
        if (content[1] == '/')                          // Break at the last line
            break;
        
        content.erase(0, 1);                                // Get rid of the tab
        size_t firstSpace = content.find_first_of(" ", 0);  // Find the first space
        space = content.find_first_of(" ", firstSpace);     // Find the second space
        
        
        // Extract the name
            size_t quote_b = content.find_first_of("\"", space);
            size_t quote_e = content.find_first_of("\"", quote_b + 1);
            
            // Resize and copy the string
            si.name.resize(quote_e - quote_b);
            std::copy(content.begin() + quote_b + 1, content.begin() + quote_e,
                      si.name.begin());
            
            // Remove the extension of the file
            si.name.erase(si.name.size() - 5, 4);
        
        
        // Extract the u coordinate
            space = quote_e + 1;
            quote_b = content.find_first_of("\"", space);
            quote_e = content.find_first_of("\"", quote_b + 1);
            
            // Convert the string to a float
            std::string u;
            std::copy(content.begin() + quote_b + 1, content.begin() + quote_e,
                      u.begin());
            si.u = atof(u.c_str()) / width;
        
        
        // Extract the v coordinate
            space = quote_e + 1;
            quote_b = content.find_first_of("\"", space);
            quote_e = content.find_first_of("\"", quote_b + 1);
            
            // Convert the string to a float
            std::string v;
            std::copy(content.begin() + quote_b + 1, content.begin() + quote_e,
                      v.begin());
            si.v = atof(v.c_str()) / height;
        
        
        // Extract the width
            space = quote_e + 1;
            quote_b = content.find_first_of("\"", space);
            quote_e = content.find_first_of("\"", quote_b + 1);
            
            // Convert the string to a float
            std::string w;
            std::copy(content.begin() + quote_b + 1, content.begin() + quote_e,
                      w.begin());
            si.width = atof(w.c_str()) / width;
        
        
        // Extract the height
            space = quote_e + 1;
            quote_b = content.find_first_of("\"", space);
            quote_e = content.find_first_of("\"", quote_b + 1);
            
            // Convert the string to a float
            std::string h;
            std::copy(content.begin() + quote_b + 1, content.begin() + quote_e,
                      h.begin());
            si.height = atof(h.c_str()) / height;
        
        info.push_back(si);
    }
    
    ipf.close();
}

void SpriteSheet::fillSprites(std::vector<Sprite>& vec, GLuint textureID, float size) {
    for (size_t i = 0; i < info.size(); i++) {
        Sprite temp(textureID, info[i].u, info[i].v, info[i].width, info[i].height, size);
        vec.push_back(temp);
    }
}

void SpriteSheet::fillEntities(std::vector<Entity>& obj, GLuint textureID, float size) {
    // Get Sprites
    std::vector<Sprite> vec;
    fillSprites(vec, textureID, size);
    
    // Fill Entities
    for (size_t i = 0; i < vec.size(); i++) {
        Entity temp;
        temp.width = vec[i].texWidth;
        temp.height = vec[i].texHeight;
        temp.sprite = &vec[i];
        obj.push_back(temp);
    }
}






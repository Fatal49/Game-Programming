#include "SpriteSheet.h"

SpriteSheet::SpriteSheet() {}

SpriteSheet::SpriteSheet(unsigned int textureID, float u, float v,
                         float width, float height, float size)
: textureID(textureID), u(u), v(v), width(width), height(height), size(size) { init(); }

SpriteSheet::SpriteSheet(const SpriteSheet& rhs)
: textureID(rhs.textureID), u(rhs.u), v(rhs.v), width(rhs.width), height(rhs.height), size(rhs.size),
    model(rhs.model), view(rhs.view), projection(rhs.projection) {
        collBox.topRight.x = rhs.collBox.topRight.x;
        collBox.topRight.y = rhs.collBox.topRight.y;
        
        collBox.topLeft.x = rhs.collBox.topLeft.x;
        collBox.topLeft.y = rhs.collBox.topLeft.y;
        
        collBox.bottomRight.x = rhs.collBox.bottomRight.x;
        collBox.bottomRight.y = rhs.collBox.bottomRight.y;
        
        collBox.bottomLeft.x = rhs.collBox.bottomLeft.x;
        collBox.bottomLeft.y = rhs.collBox.bottomLeft.y;
        
        try {
            for (int i = 0; i < 12; i++) {
                vertices[i] = rhs.vertices[i];
                texCoords[i] = rhs.texCoords[i];
            }
        } catch (...) {
            printf("There was a problem\n");
        }

}

void SpriteSheet::init() {
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
    
    collBox.topLeft = vec::vec2(vertices[4], vertices[5]);
    collBox.topRight = vec::vec2(vertices[2], vertices[3]);
    collBox.bottomLeft = vec::vec2(vertices[0], vertices[1]);
    collBox.bottomRight = vec::vec2(vertices[10], vertices[11]);}

void SpriteSheet::draw(Shader *program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set matrices
    program->setModelMatrix(model);
    program->setViewMatrix(view);
    program->setProjectionMatrix(projection);
    
    model.identity();
    view.identity();
    projection.identity();
    
    // Draw sprite
    glVertexAttribPointer(program->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glEnableVertexAttribArray(program->getPositionAttrib());
    glVertexAttribPointer(program->getTexCoordAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
    glEnableVertexAttribArray(program->getTexCoordAttrib());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->getPositionAttrib());
    glDisableVertexAttribArray(program->getTexCoordAttrib());
}
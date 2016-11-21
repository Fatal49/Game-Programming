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


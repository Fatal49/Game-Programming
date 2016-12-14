#include "SpriteSheet.hpp"

Sprite::Sprite() {}

Sprite::Sprite(GLuint textureID, float u, float v,
                         float width, float height, float tw, float th, float openGlX,
                         float openGlY, float w, float h, float s)
: textureID(textureID), u(u), v(v), width(width), height(height), tw(tw), th(th), openGlX(openGlX),
openGlY(openGlY), w(w), h(h), size(s) { init(); }

Sprite::Sprite(const Sprite& rhs)
: textureID(rhs.textureID), u(rhs.u), v(rhs.v), width(rhs.width), height(rhs.height), tw(rhs.tw),
model(rhs.model), view(rhs.view), projection(rhs.projection), th(rhs.th), openGlY(rhs.openGlY),
openGlX(rhs.openGlX), w(rhs.w), h(rhs.h), size(rhs.size) {
    try {
        for (int i = 0; i < 12; i++) {
            vertices[i] = rhs.vertices[i];
            texCoords[i] = rhs.texCoords[i];
        }
    } catch (...) {
        printf("Sprite: There was a problem\n");
    }
}

void Sprite::init() {
    float TS_WIDTH = openGlX / (w / tw);
    float TS_HEIGHT = openGlY / (h / th);
    
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
    
//    texCoords[0] = u;                        // Top Left
//    texCoords[1] = v;
//    texCoords[2] = u;                        // Bottom Left
//    texCoords[3] = v + height;
//    texCoords[4] = u + width;                // Top Right
//    texCoords[5] = v;
//    texCoords[6] = u;                        // Bottom Left
//    texCoords[7] = v + height;
//    texCoords[8] = u + width;                // Bottom Right
//    texCoords[9] = v + height;
//    texCoords[10] = u + width;               // Top Right
//    texCoords[11] = v;
    
    // Aspect ratio
    float aspect = width / height;
    
    // Sprite vertices    
//    vertices[0] = 0;                        // Top Left
//    vertices[1] = 0;
//    vertices[2] = 0;                        // Bottom Left
//    vertices[3] = -TS_HEIGHT;
//    vertices[4] = TS_WIDTH;                 // Top Right
//    vertices[5] = 0;
//    vertices[6] = 0;                        // Bottom Left
//    vertices[7] = -TS_HEIGHT;
//    vertices[8] = TS_WIDTH;                 // Bottom Right
//    vertices[9] = -TS_HEIGHT;
//    vertices[10] = TS_WIDTH;                // Top Right
//    vertices[11] = 0;
    
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











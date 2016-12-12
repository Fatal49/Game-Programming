#include "BulletEmitter.hpp"

BulletEmitter::BulletEmitter(float w, float h, vec::vec2 v, unsigned int maxBullets)
: maxBullets(maxBullets), velocity(v), width(w), height(h), angle(0.0f),
scaling(vec::vec2(1.0f, 1.0f)) {}

BulletEmitter::BulletEmitter(unsigned int maxBullets) : maxBullets(maxBullets) {}

BulletEmitter::BulletEmitter(const BulletEmitter& rhs)
: maxBullets(rhs.maxBullets), velocity(rhs.velocity), position(rhs.position),
textureID(rhs.textureID), matrix(rhs.matrix), width(rhs.width), height(rhs.height),
angle(rhs.angle), scaling(rhs.scaling) {
    // Clear the containers
    bullets.clear();
    vertices.clear();
    indices.clear();
    texCoord.clear();
    
    // Get all of rhs's data held in the containers
    for (size_t i = 0; i < rhs.vertices.size(); i++)
        vertices[i] = rhs.vertices[i];
    
    for (size_t i = 1; i < rhs.texCoord.size(); i++)
        texCoord[i] = rhs.texCoord[i];
    
    for (size_t i = 1; i < rhs.indices.size(); i++)
        indices[i] = rhs.indices[i];
    
    for (size_t i = 0; i < rhs.points.size(); i++)
        points[i] = rhs.points[i];
    
    for (size_t i = 0; i < rhs.bullets.size(); i++)
        bullets.push_back(Bullet(rhs.bullets[i].position, rhs.bullets[i].alive));
}

BulletEmitter::~BulletEmitter() {}

BulletEmitter& BulletEmitter::operator=(const BulletEmitter &rhs) {
    if (this != &rhs) {
        // Get rhs's data
        velocity = rhs.velocity;
        position = rhs.position;
        maxBullets = rhs.maxBullets;
        textureID = rhs.textureID;
        matrix = rhs.matrix;
        width = rhs.width;
        height = rhs.height;
        angle = rhs.angle;
        scaling = rhs.scaling;
        
        // Clear the containers
        bullets.clear();
        vertices.clear();
        indices.clear();
        texCoord.clear();
        
        // Get all of rhs's data held in the containers
        for (size_t i = 0; i < rhs.vertices.size(); i++)
            vertices[i] = rhs.vertices[i];
        
        for (size_t i = 1; i < rhs.texCoord.size(); i++)
            texCoord[i] = rhs.texCoord[i];
        
        for (size_t i = 1; i < rhs.indices.size(); i++)
            indices[i] = rhs.indices[i];
        
        for (size_t i = 0; i < rhs.points.size(); i++)
            points[i] = rhs.points[i];
        
        for (size_t i = 0; i < rhs.bullets.size(); i++)
            for (size_t i = 0; i < rhs.bullets.size(); i++)
                bullets.push_back(Bullet(rhs.bullets[i].position, rhs.bullets[i].alive));
    }
    
    return *this;
}

void BulletEmitter::setVelocity(vec::vec2 v) { velocity = v; }

float BulletEmitter::getWidth() { return width; }

float BulletEmitter::getHeight() { return height; }

void BulletEmitter::scale(float x, float y) {
    scaling.x = x;
    scaling.y = y;
}

void BulletEmitter::translate(float x, float y) {
    position.x += x;
    position.y += y;
}

void BulletEmitter::rotate(float angle) { this->angle = angle; }

void BulletEmitter::createBullets() {
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    vertices.clear();
    indices.clear();
    texCoord.clear();
    
    vertices.insert(vertices.end(), {
        -halfW, halfH,          // Top Left
        halfW, halfH,           // Top Right
        -halfW, -halfH,         // Bottom Left
        halfW, -halfH           // Bottom Right
    });
    
    indices.insert(indices.end(), {
        0, 1, 2,                // Triangle on the right
        3, 2, 1                 // Triangle on the left
    });
    
    texCoord.insert(texCoord.end(), {
        0, 0,                   // Triangle on the right
        1, 0,
        0, 1,
        1, 1,                   // Triangle on the left
        0, 1,
        1, 0
    });
    
    points.insert(points.end(), {
        vec::vec2(vertices[0], vertices[1]),    // Top Left
        vec::vec2(vertices[2], vertices[3]),    // Top Right
        vec::vec2(vertices[6], vertices[7]),    // Bottom Right
        vec::vec2(vertices[4], vertices[5])     // Bottom Left
    });
}

void BulletEmitter::LoadTexture(const char *image_path) {
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

bool BulletEmitter::shootABullet() {
    // If the maxBullet count has been reached do not add anymore bullets
    if (bullets.size() == maxBullets)
        return false;
    
    // If the vector is empty add a bullet
    if (bullets.size() == 0)
        bullets.push_back(Bullet(position, true));
    else {
        // Look through the vector for a dead bullet
        for (size_t i = 0; i < bullets.size(); i++) {
            // Once found bring the bullet back to life
            if (!bullets[i].alive) {
                bullets[i].alive = true;
                bullets[i].position = position;
                return true;
            }
        }
        
        // If no bullets are dead add another bullet
        bullets.push_back(Bullet(position, true));
    }
    
    return true;
}

void BulletEmitter::draw(Shader* shader) {
    for (size_t i = 0; i < bullets.size(); i++) {
        matrix.identity();
        matrix.Translate(bullets[i].position.x, bullets[i].position.y, 0.0f);
        matrix.Scale(scaling.x, scaling.y, 1.0f);
        matrix.Rotate(angle);
        
        shader->setModelMatrix(matrix);
        shader->bind();
        
        glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
        glEnableVertexAttribArray(shader->getPositionAttrib());
        glVertexAttribPointer(shader->getTexCoordAttrib(), 2, GL_FLOAT, false, 0, &texCoord[0]);
        glEnableVertexAttribArray(shader->getTexCoordAttrib());
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_BYTE, &indices[0]);
        
        glDisableVertexAttribArray(shader->getPositionAttrib());
        glDisableVertexAttribArray(shader->getTexCoordAttrib());
        glBindTexture(GL_TEXTURE_2D, 0);
        
        shader->unbind();
    }
}

void BulletEmitter::update(float elapsed) {
    // Update all bullets that are alive
    for (size_t i = 0; i < bullets.size(); i++)
        if (bullets[i].alive)
            bullets[i].position += velocity;
}








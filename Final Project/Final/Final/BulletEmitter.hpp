#ifndef __BULLETEMITTER_HPP__
#define __BULLETEMITTER_HPP__

#include <stdio.h>
#include "Shader.hpp"
#include "vec.h"
#include <vector>
#include <SDL2_image/SDL_image.h>

struct Bullet {
    Bullet() {}
    Bullet(vec::vec2 p, bool alive = false): position(p), alive(alive) {}
    
    vec::vec2 position;
    bool alive;
};

class BulletEmitter {
public:
    
    // Constructors and deconstructors
    BulletEmitter(float w, float h, unsigned int maxBullets, vec::vec2 v);
    BulletEmitter(unsigned int maxBullets);
    BulletEmitter(const BulletEmitter& rhs);
    ~BulletEmitter();
    
    // Operator Overload
    BulletEmitter& operator = (const BulletEmitter& rhs);
    
    // Data
    vec::vec2 position;
    Matrix matrix;
    
    // Methods
    void setVelocity(vec::vec2 v);
    float getWidth();
    float getHeight();
    
    void scale(float x, float y);
    void translate(float x, float y);
    void rotate(float angle);
    
    void createBullets();
    void LoadTexture(const char *image_path);
    bool shootABullet();
    void draw(Shader* shader);
    void update(float elapsed);
    
private:
    
    // ID's
    GLuint textureID;
    
    // Data
    float width;
    float height;
    float angle;
    vec::vec2 velocity;
    vec::vec2 scaling;
    unsigned int maxBullets;
    
    // Containers
    std::vector<Bullet> bullets;
    std::vector<GLfloat> vertices;
    std::vector<GLubyte> indices;
    std::vector<GLfloat> texCoord;
    std::vector<vec::vec2> points;
    
};


#endif /* __BULLETEMITTER_HPP__ defined */

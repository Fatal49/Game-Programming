#ifndef __BULLETEMITTER_HPP__
#define __BULLETEMITTER_HPP__

#include <stdio.h>
#include "ShaderProgram.h"
#include "vec.h"
#include <vector>
#include <SDL2_image/SDL_image.h>

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

enum direction { TOP, RIGHT, BOTTOM, LEFT };

struct Bullet {
    Bullet() {}
    Bullet(vec::vec2 p, bool alive = false): position(p), alive(alive) {}
    
    vec::vec2 position;
    bool alive;
};

class BulletEmitter {
public:
    
    // Constructors and deconstructors
    BulletEmitter(float w, float h, vec::vec2 v = vec::vec2(0.0f), unsigned int maxBullets = 10);
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
    void draw(ShaderProgram* shader);
    void update(float elapsed);
    
private:
    
    // Methods
    bool viewCheck(Matrix mvp);
    
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

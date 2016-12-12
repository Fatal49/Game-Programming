
#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__

#include "BulletEmitter.hpp"

enum direction { TOP, RIGHT, BOTTOM, LEFT };

class Rectangle {
public:
    
    // Constructors and Deconstructor
    Rectangle(float w, float h);
    Rectangle(const Rectangle& rhs);
    
    // Physics info
    vec::vec2 velocity;
    
    // Methods
    void create();
    void draw(Shader* shader);
    void update(float elapsed);
    
    void translate(float x, float y);
    void scale(float x, float y);
    void rotate(float angle);
        
    const std::vector<vec::vec2> getPoints();
    float getWidth() const;
    float getHeight() const;
    vec::vec2 getCenter();
    
    void LoadTexture(const char *image_path);
    void setupBulletEmitter(float width, float height, float rotate,
                            vec::vec2 scale, const char* textureFileName,
                            direction shootingDirection,
                            unsigned int maxBullets = 10);
    void shootABullet();
    
private:
    
    // Id's
    GLuint textureID;
    
    // Dimensions
    float width;
    float height;
    
    // Data
    Matrix matrix;
    Matrix rotating;
    vec::vec2 scaling;
    float angle;
    
    // Object info
    vec::vec2 position;
    std::vector<GLfloat> vertices;
    std::vector<GLubyte> indices;
    std::vector<GLfloat> texCoord;
    std::vector<vec::vec2> points;
    
    // BulletEmitter
    BulletEmitter* be;
    
};

#endif /* __RECTANGLE_HPP__ defined */

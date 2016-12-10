
#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__

#include <stdio.h>
#include "vec.h"
#include "Shader.hpp"
#include <vector>
#include <SDL2/SDL_opengl.h>

class Rectangle {
public:
    
    // Constructors and Deconstructor
    Rectangle(float w, float h);
    Rectangle(const Rectangle& rhs);
    
    // Physics info
    vec::vec2 velocity;
    
    // Flags
    bool colliding = false;
    bool lines = false;    
    
    // Methods
    void create();
    void draw(Shader* shader);
    void drawLines();
    void update(float elapsed);
    
    void translate(float x, float y);
    void scale(float x, float y);
    void rotate(float angle);
        
    const std::vector<vec::vec2> getPoints();
    float getWidth() const;
    float getHeight() const;
    vec::vec2 getCenter();
    
private:
    
    // Dimensions
    float width;
    float height;
    
    // Data
    Matrix matrix;
    Matrix rotating;
    vec::vec2 responseVec;
    vec::vec2 scaling;
    float angle;
    
    // Object info
    vec::vec2 position;
    GLfloat vertices[10];
    GLubyte indices[12];
    GLfloat color[20];
    std::vector<vec::vec2> points;
    
    // Debug info
    GLfloat v[8];
    GLubyte i[6];
    GLfloat c[16];
    
};

#endif /* __RECTANGLE_HPP__ defined */

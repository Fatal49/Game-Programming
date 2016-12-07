
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
    vec::vec2 position;
    
    // Methods
    void create();
    void draw(Shader* shader);
    void update(float elapsed);
    
    void translate(float x, float y);
    void scale(float x, float y);
    void rotate(float angle);
        
    const std::vector<vec::vec2> getPoints();    
    
private:
    
    // Dimensions
    float width;
    float height;
    
    // Data
    Matrix model;
    Matrix scaling;
    Matrix rotating;
    
    // Object info
    GLfloat vertices[10];
    GLubyte indices[12];
    GLfloat color[20];
    std::vector<vec::vec2> points;
};

#endif /* __RECTANGLE_HPP__ defined */

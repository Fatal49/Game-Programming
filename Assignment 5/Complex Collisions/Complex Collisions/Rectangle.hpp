
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
    
    // Methods
    void create();
    void debug();
    void draw(Shader* shader);
    void update(float elapsed);
    
    void translate(float x, float y);
    void scale(float x, float y);
    void rotate(float angle);
    
    void setVelocity(vec::vec2 v);
    const std::vector<vec::vec2> getPoints();
    const std::vector<vec::vec2> getEdges();
    bool checkCollision(const std::vector<vec::vec2> points);

private:
    
    // Methods
    bool testSATSeparationForEdge(float edgeX, float edgeY,
                                  const std::vector<vec::vec2> &points1,
                                  const std::vector<vec::vec2> &points2);
    bool checkSATCollision(const std::vector<vec::vec2> &e1Points,
                           const std::vector<vec::vec2> &e2Points);
    
    // Debug info
    std::vector<GLfloat> vert;
    std::vector<GLubyte> i;
    std::vector<GLfloat> c;
    bool debugFlag = false;
    
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
    
    // Physics info
    vec::vec2 position;
    vec::vec2 velocity;
    
    // Flags
    
    
    
};

#endif /* __RECTANGLE_HPP__ defined */

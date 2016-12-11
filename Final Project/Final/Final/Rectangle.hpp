
#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__

#include <stdio.h>
#include "vec.h"
#include "Shader.hpp"
#include <vector>
#include <SDL2_image/SDL_image.h>

class Rectangle {
public:
    
    // Constructors and Deconstructor
    Rectangle(float w, float h, bool textured = false);
    Rectangle(const Rectangle& rhs);
    
    // Physics info
    vec::vec2 velocity;
    
    // Flags
    bool colliding;
    bool texture;
    
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
    std::vector<GLfloat> data;
    std::vector<vec::vec2> points;
    
};

#endif /* __RECTANGLE_HPP__ defined */

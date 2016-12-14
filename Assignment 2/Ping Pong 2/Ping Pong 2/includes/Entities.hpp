#ifndef __ENTITIES_HPP__
#define __ENTITIES_HPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include "Matrix.hpp"
#include "gl_macros.hpp"

struct point { float x; float y; float z; };

struct _vec {
    point position;
    float angle;
};

class Entity : public Matrix {
private:

    // Dimensions & mass of the entity
    float width = 0.0f;
    float height = 0.0f;
    float mass = 0.0f;
    
    // Center point of the entity
    point center;
    
    // Function to calculate center of the entity
    point calculateCenter();
    
public:
    
    const float up = 1.0f;
    const float down = -1.0f;
    
    // Vertices, Indices & vector
    std::vector<Vertex> vertices;
    std::vector<GLubyte> indices;    
    _vec vector;
    float xVel = 0.0f;
    float yVel = 0.0f;
    
    // Borders of entity
    point topLeft, topRight, bottomRight, bottomLeft;

    // Constructors
    Entity();
    Entity(const std::vector<Vertex>& __vert, const std::vector<GLubyte>& __indi, float _w, float _h);

    // Getters
    Matrix getViewMatrix(); 
    Matrix getModelMatrix(); 
    Matrix getProjectionMatrix();
    float getWidth() const;
    float getHeight() const;
    point getCenter();
    float getMass();

    // Setters
    void setVectors(std::vector<Vertex>& __vert, std::vector<GLubyte>& __indi);
    void setModelMatrix(Matrix __m);
    void setViewMatrix(Matrix __v);
    void setProjectionMatrix(Matrix __p);
    void setWidth(float _w);
    void setHeight(float _h);
    void setMass(float _m);

    // Other methods
    size_t vertexSizeInBytes(); 
    size_t indiceSizeInBytes();
    void reloadCenter();
    void changeX(const float x);
    void changeY(const float y);
    
    // Matrices of the object
    Matrix view, model, projection;
    
};

#endif /* __ENTITIES_HPP__ defined */

#ifndef __ENTITIES_HPP__
#define __ENTITIES_HPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include "Matrix.hpp"
#include "gl_macros.hpp"

class Entity : public Matrix {
private:

    // Dimensions of the entity
    float width = 0.0f;
    float height = 0.0f;

public:
    
    // Vertices of the entity
    std::vector<Vertex> vertices;

    // Indices of the entity
    std::vector<GLubyte> indices;

    // Constructors
    Entity();
    Entity(std::vector<Vertex>& __vert, std::vector<GLubyte>& __indi, float _w, float _h);

    // Getters
    Matrix getViewMatrix(); 
    Matrix getModelMatrix(); 
    Matrix getProjectionMatrix(); 
    Vertex* getVertexData();
    GLubyte* getIndicesData();
    float getWidth() const;
    float getHeight() const;
    float getX() const;
    float getY() const;
    float getZ() const;

    // Setters
    void setVectors(std::vector<Vertex>& __vert, std::vector<GLubyte>& __indi);
    void setModelMatrix(Matrix __m);
    void setViewMatrix(Matrix __v);
    void setProjectionMatrix(Matrix __p);
    void setWidth(float _w);
    void setHeight(float _h);

    size_t vertexSizeInBytes(); 
    size_t indiceSizeInBytes(); 
    
    // Matrices of the object
    Matrix view, model, projection;

};

#endif /* __ENTITIES_HPP__ defined */

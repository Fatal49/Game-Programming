#include "Entities.hpp"

Entity::Entity() {}

Entity::Entity(std::vector<Vertex>& __vert, std::vector<GLubyte>& __indi, 
        float _w, float _h) : width(_w), height(_h) {
    for (int i = 0; i < __vert.size(); i++)
        vertices.push_back(__vert[i]);

    for (int i = 0; i < __indi.size(); i++)
        indices.push_back(__indi[i]);
}

Matrix Entity::getViewMatrix() { return view; }
Matrix Entity::getModelMatrix() { return model; }
Matrix Entity::getProjectionMatrix() { return projection; }
Vertex* Entity::getVertexData() { return &vertices[0]; }
GLubyte* Entity::getIndicesData() { return &indices[0]; }
float Entity::getWidth() const { return width; }
float Entity::getHeight() const { return height; }
float Entity::getX() const { return model.m[3][0]; }
float Entity::getY() const { return model.m[3][1]; }
float Entity::getZ() const { return model.m[3][2]; }

void Entity::setVectors(std::vector<Vertex>& __vert, std::vector<GLubyte>& __indi) {
    for (int i = 0; i < __vert.size(); i++)
        vertices.push_back(__vert[i]);

    for (int i = 0; i < __indi.size(); i++)
        indices.push_back(__indi[i]);
}

void Entity::setModelMatrix(Matrix __m) { model = __m; }
void Entity::setViewMatrix(Matrix __v) { view = __v; }
void Entity::setProjectionMatrix(Matrix __p) { projection = __p; }
void Entity::setWidth(float _w) { width = _w; }
void Entity::setHeight(float _h) { height = _h; }

size_t Entity::vertexSizeInBytes() { return vertices.size() * sizeof(Vertex); }
size_t Entity::indiceSizeInBytes() { return indices.size() * sizeof(GLubyte); }

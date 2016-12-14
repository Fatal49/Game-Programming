#include "Entities.hpp"

Entity::Entity() {}

Entity::Entity(const std::vector<Vertex>& __vert, const std::vector<GLubyte>& __indi,
        float _w, float _h) : width(_w), height(_h) {
    for (int i = 0; i < __vert.size(); i++)
        vertices.push_back(__vert[i]);

    for (int i = 0; i < __indi.size(); i++)
        indices.push_back(__indi[i]);
    
    center = calculateCenter();
}

point Entity::calculateCenter() {
    point temp;
    float _x = 0.0f, _y = 0.0f;
    
    for (size_t i = 0; i < indices.size(); i++) {
        _x += vertices[i].position[0];
        _y += vertices[i].position[1];
    }
    
    temp.x = (_x / (float) indices.size()) + model.m[3][0];
    temp.y = (_y / (float) indices.size()) + model.m[3][1];
    temp.z = model.m[3][2];
    
    return temp;
}


// Getter
Matrix Entity::getViewMatrix() { return view; }
Matrix Entity::getModelMatrix() { return model; }
Matrix Entity::getProjectionMatrix() { return projection; }
float Entity::getWidth() const { return width; }
float Entity::getHeight() const { return height; }
point Entity::getCenter() { return center; }
float Entity::getMass() { return mass; }


// Setter
void Entity::setVectors(std::vector<Vertex>& __vert, std::vector<GLubyte>& __indi) {
    for (int i = 0; i < __vert.size(); i++)
        vertices.push_back(__vert[i]);

    for (int i = 0; i < __indi.size(); i++)
        indices.push_back(__indi[i]);
    
    center = calculateCenter();
}
void Entity::setModelMatrix(Matrix __m) { model = __m; }
void Entity::setViewMatrix(Matrix __v) { view = __v; }
void Entity::setProjectionMatrix(Matrix __p) { projection = __p; }
void Entity::setWidth(float _w) { width = _w; }
void Entity::setHeight(float _h) { height = _h; }
void Entity::setMass(float _m) { mass = _m; }


size_t Entity::vertexSizeInBytes() { return vertices.size() * sizeof(Vertex); }
size_t Entity::indiceSizeInBytes() { return indices.size() * sizeof(GLubyte); }
void Entity::reloadCenter() { center = calculateCenter(); }
void Entity::changeX(const float x) { model.m[3][0] *= x; }
void Entity::changeY(const float y) { model.m[3][1] *= y; }










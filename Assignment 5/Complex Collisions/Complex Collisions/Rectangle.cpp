
#include "Rectangle.hpp"

Rectangle::Rectangle(float w, float h) : width(w), height(h) {}

Rectangle::Rectangle(const Rectangle& rhs): width(rhs.width), height(rhs.height),
    position(rhs.position), model(rhs.model), scaling(rhs.scaling), rotating(rhs.rotating)
{
    try {
        // Get the vertices
        for (int i = 0; i < 12; i++)
            vertices[i] = rhs.vertices[i];
        
        // Get the colors
        for (int i = 1; i < 16; i++)
            color[i] = rhs.color[i];
        
        // Get the points
        for (int i = 0; i < rhs.points.size(); i++)
            points[i] = rhs.points[i];
        
    } catch (...) {
        printf("Rectangle(const rhs& Reactangle): error\n");
    }
}

void Rectangle::create() {
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    // Vertices
    vertices[0] = -halfW;       // Top Left
    vertices[1] = halfH;
    vertices[2] = halfW;        // Top Right
    vertices[3] = halfH;
    vertices[4] = 0;            // Center
    vertices[5] = 0;
    vertices[6] = -halfW;       // Bottom Left
    vertices[7] = -halfH;
    vertices[8] = halfW;       // Bottom Right
    vertices[9] = -halfH;
    
    // Indices
    indices[0] = 0;            // Triangle on the top
    indices[1] = 1;
    indices[2] = 2;
    
    indices[3] = 0;            // Triangle on the right
    indices[4] = 2;
    indices[5] = 3;
    
    indices[6] = 1;            // Triangle on the left
    indices[7] = 2;
    indices[8] = 4;
    
    indices[9] = 3;            // Triangle on the bottom
    indices[10] = 2;
    indices[11] = 4;
    
    // Color
    color[0] = 1.0f;            // Top Left
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
    
    color[4] = 0.0f;            // Top Right
    color[5] = 1.0f;
    color[6] = 0.0f;
    color[7] = 1.0f;
    
    color[8] = 0.0f;            // Center
    color[9] = 0.0f;
    color[10] = 1.0f;
    color[11] = 1.0f;
    
    color[12] = 0.0f;            // Bottom Left
    color[13] = 1.0f;
    color[14] = 0.0f;
    color[15] = 1.0f;
    
    color[16] = 1.0f;            // Bottom Right
    color[17] = 0.0f;
    color[18] = 0.0f;
    color[19] = 1.0f;
    
    // Set the points of the each vertex
    points.insert(points.end(), {
        vec::vec2(vertices[0], vertices[1]),    // Top Left
        vec::vec2(vertices[2], vertices[3]),    // Top Right
        vec::vec2(vertices[8], vertices[9]),    // Bottom Right
        vec::vec2(vertices[6], vertices[7])     // Bottom Left
    });
}

void Rectangle::draw(Shader* shader) {
    model.identity();
    model.Translate(position.x, position.y, 0.0f);
    model.Scale(scaling.x, scaling.y, 1.0f);
    model.Rotate(angle);
    
    shader->setModelMatrix(model);
    shader->bind();
    
    glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glEnableVertexAttribArray(shader->getPositionAttrib());
    
    glVertexAttribPointer(shader->getColorAttrib(), 4, GL_FLOAT, GL_FALSE, 0, &color[0]);
    glEnableVertexAttribArray(shader->getColorAttrib());
    
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, &indices[0]);
    
    glDisableVertexAttribArray(shader->getPositionAttrib());
    glDisableVertexAttribArray(shader->getColorAttrib());
    
    shader->unbind();
}

void Rectangle::update(float elapsed) {
    translate(velocity.x * elapsed, velocity.y * elapsed);
}

void Rectangle::translate(float x, float y) {
    position.x += x;
    position.y += y;
}

void Rectangle::scale(float x, float y) {
    scaling.x = x;
    scaling.y = y;
}

void Rectangle::rotate(float angle) {
    rotating.m[0][0] = cos(angle);
    rotating.m[1][0] = -sin(angle);
    rotating.m[0][1] = sin(angle);
    rotating.m[1][1] = cos(angle);
    
    this->angle += angle;
}

const std::vector<vec::vec2> Rectangle::getPoints() {
    std::vector<vec::vec2> p;
    Matrix t, s;

    t.m[0][3] = position.x;
    t.m[1][3] = position.y;
    
    s.m[0][0] = scaling.x;
    s.m[1][1] = scaling.y;
    
    Matrix all = t * s * rotating;
    
    for (int i = 0; i < points.size(); i++) {
        vec::vec4 v(points[i].x, points[i].y, 1.0f, 1.0f);
        vec::vec4 finalPos = all * v;
        p.push_back(vec::vec2(finalPos.x, finalPos.y));
    }

    return p;
}









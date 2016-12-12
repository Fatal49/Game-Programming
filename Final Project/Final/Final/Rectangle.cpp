
#include "Rectangle.hpp"

Rectangle::Rectangle(float w, float h, bool textured) : width(w), height(h),
colliding(false), texture(textured), scaling(vec::vec2(1.0f, 1.0f)) {}

Rectangle::Rectangle(const Rectangle& rhs): width(rhs.width), height(rhs.height),
position(rhs.position), matrix(rhs.matrix), scaling(rhs.scaling), rotating(rhs.rotating),
angle(rhs.angle), velocity(rhs.velocity), colliding(rhs.colliding), texture(rhs.texture),
textureID(rhs.textureID)
{
    try {
        // Get the vertices
        for (size_t i = 0; i < rhs.vertices.size(); i++)
            vertices[i] = rhs.vertices[i];
        
        // Get the colors
        for (size_t i = 1; i < rhs.data.size(); i++)
            data[i] = rhs.data[i];
        
        // Get the indices
        for (size_t i = 1; i < rhs.indices.size(); i++)
            indices[i] = rhs.indices[i];
        
        // Get the points
        for (size_t i = 0; i < rhs.points.size(); i++)
            points[i] = rhs.points[i];
        
    } catch (...) {
        printf("Rectangle(const rhs& Reactangle): error\n");
    }
}

void Rectangle::create() {
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    if (!texture) {
        vertices.clear();
        indices.clear();
        data.clear();
        
        vertices.insert(vertices.end(), {
            -halfW, halfH,          // Top Left
            halfW, halfH,           // Top Right
            0, 0,                   // Center
            -halfW, -halfH,         // Bottom Left
            halfW, -halfH           // Bottom Right
        });
        
        indices.insert(indices.end(), {
            0, 1, 2,                // Triangle on the top
            0, 2, 3,                // Triangle on the right
            1, 2, 4,                // Triangle on the left
            3, 2, 4                 // Triangle on the bottom
        });
        
        data.insert(data.end(), {
            1.0f, 0.0f, 0.0f, 1.0f,         // Top Left
            0.0f, 1.0f, 0.0f, 1.0f,         // Top Right
            0.0f, 0.0f, 1.0f, 1.0f,         // Center
            0.0f, 1.0f, 0.0f, 1.0f,         // Bottom Left
            1.0f, 0.0f, 0.0f, 1.0f          // Bottom Right
        });
        
        points.insert(points.end(), {
            vec::vec2(vertices[0], vertices[1]),    // Top Left
            vec::vec2(vertices[2], vertices[3]),    // Top Right
            vec::vec2(vertices[8], vertices[9]),    // Bottom Right
            vec::vec2(vertices[6], vertices[7])     // Bottom Left
        });
    } else  {
        vertices.clear();
        indices.clear();
        data.clear();
        
        vertices.insert(vertices.end(), {
            -halfW, halfH,          // Top Left
            halfW, halfH,           // Top Right
            -halfW, -halfH,         // Bottom Left
            halfW, -halfH           // Bottom Right
        });
        
        indices.insert(indices.end(), {
            0, 1, 2,                // Triangle on the right
            3, 2, 1                 // Triangle on the left
        });
        
        data.insert(data.end(), {
            0, 0,                   // Triangle on the right
            1, 0,
            0, 1,
            1, 1,                   // Triangle on the left
            0, 1,
            1, 0
        });
        
        points.insert(points.end(), {
            vec::vec2(vertices[0], vertices[1]),    // Top Left
            vec::vec2(vertices[2], vertices[3]),    // Top Right
            vec::vec2(vertices[6], vertices[7]),    // Bottom Right
            vec::vec2(vertices[4], vertices[5])     // Bottom Left
        });
    }

}

void Rectangle::draw(Shader* shader) {
    matrix.identity();
    matrix.Translate(position.x, position.y, 0.0f);
    matrix.Scale(scaling.x, scaling.y, 1.0f);
    matrix.Rotate(angle);
    
    shader->setModelMatrix(matrix);
    shader->bind();
    
    glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glEnableVertexAttribArray(shader->getPositionAttrib());
    
    if (texture) {
        glVertexAttribPointer(shader->getTexCoordAttrib(), 2, GL_FLOAT, false, 0, &data[0]);
        glEnableVertexAttribArray(shader->getTexCoordAttrib());
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glVertexAttribPointer(shader->getColorAttrib(), 4, GL_FLOAT, GL_FALSE, 0, &data[0]);
        glEnableVertexAttribArray(shader->getColorAttrib());
    }
    
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_BYTE, &indices[0]);
    
    glDisableVertexAttribArray(shader->getPositionAttrib());
    if (texture) {
        glDisableVertexAttribArray(shader->getTexCoordAttrib());
        glBindTexture(GL_TEXTURE_2D, 0);
    } else
        glDisableVertexAttribArray(shader->getColorAttrib());
    
    shader->unbind();
}

void Rectangle::update(float elapsed) {
    if (!colliding)
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
    this->angle += angle;
}

const std::vector<vec::vec2> Rectangle::getPoints() {
    std::vector<vec::vec2> p;
    Matrix t, s, all;

    t.m[0][3] = position.x;
    t.m[1][3] = position.y;
    
    s.m[0][0] = scaling.x;
    s.m[1][1] = scaling.y;
    
    rotating.m[0][0] = cos(angle);
    rotating.m[1][0] = sin(angle);
    rotating.m[0][1] = -sin(angle);
    rotating.m[1][1] = cos(angle);
    
    all = t * s * rotating;
    
    for (int i = 0; i < points.size(); i++) {
        vec::vec4 v(points[i].x, points[i].y, 1.0f, 1.0f);
        vec::vec4 finalPos = all * v;
        p.push_back(vec::vec2(finalPos.x, finalPos.y));
    }

    return p;
}

float Rectangle::getWidth() const { return width; }

float Rectangle::getHeight() const { return height; }

vec::vec2 Rectangle::getCenter() {
    std::vector<vec::vec2> p = getPoints();
    return vec::vec2(p[0].x + (width * 0.5), p[0].y - (height * 0.5));
}

void Rectangle::LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
}










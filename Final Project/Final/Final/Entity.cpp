
#include "Rectangle.hpp"

Rectangle::Rectangle(float w, float h) : width(w), height(h),
scaling(vec::vec2(1.0f, 1.0f)), be(nullptr) {}

Rectangle::Rectangle(const Rectangle& rhs): width(rhs.width), height(rhs.height),
position(rhs.position), matrix(rhs.matrix), scaling(rhs.scaling), rotating(rhs.rotating),
angle(rhs.angle), velocity(rhs.velocity), textureID(rhs.textureID)
{
    try {
        // Get the BulletEmitter
        if (be) {
            delete be;
            if (rhs.be)
                be = new BulletEmitter(*(rhs.be));
        }
        
        // Get the vertices
        for (size_t i = 0; i < rhs.vertices.size(); i++)
            vertices[i] = rhs.vertices[i];
        
        // Get the colors
        for (size_t i = 1; i < rhs.texCoord.size(); i++)
            texCoord[i] = rhs.texCoord[i];
        
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
    
    vertices.clear();
    indices.clear();
    texCoord.clear();
    
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
    
    texCoord.insert(texCoord.end(), {
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

void Rectangle::draw(Shader* shader) {
    matrix.identity();
    matrix.Translate(position.x, position.y, 0.0f);
    matrix.Scale(scaling.x, scaling.y, 1.0f);
    matrix.Rotate(angle);
    
    shader->setModelMatrix(matrix);
    shader->bind();
    
    glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    glEnableVertexAttribArray(shader->getPositionAttrib());
    glVertexAttribPointer(shader->getTexCoordAttrib(), 2, GL_FLOAT, false, 0, &texCoord[0]);
    glEnableVertexAttribArray(shader->getTexCoordAttrib());
    
    glBindTexture(GL_TEXTURE_2D, textureID);    
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_BYTE, &indices[0]);
    
    glDisableVertexAttribArray(shader->getPositionAttrib());
    glDisableVertexAttribArray(shader->getTexCoordAttrib());
    glBindTexture(GL_TEXTURE_2D, 0);

    shader->unbind();
    
    be->draw(shader);
}

void Rectangle::update(float elapsed) {
    translate(velocity.x * elapsed, velocity.y * elapsed);
    
    // Check if the BulletEmitter has been allocated for
    if (be) {
        be->update(elapsed);
    }
}

void Rectangle::translate(float x, float y) {
    position.x += x;
    position.y += y;
    
    if (be) {
        be->translate(x, y);
    }
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

void Rectangle::setupBulletEmitter(float width, float height, float rotate,
                                   vec::vec2 scale, const char* textureFileName,
                                   direction shootingDirection,
                                   unsigned int maxBullets) {
    if (!be) {
        be = new BulletEmitter(width, height, vec::vec2(0.0f), maxBullets);
        be->LoadTexture(textureFileName);
        be->createBullets();
        be->scale(scale.x, scale.y);
        be->rotate(rotate);
        
        // Set the shooting position and the velocity
        switch (shootingDirection) {
            case TOP:
            {
                be->position = vec::vec2(position.x, position.y + height);
                be->setVelocity(vec::vec2(0.0f, 0.15f));
            } break;
                
            case RIGHT:
            {
                be->position = vec::vec2(position.x + width, position.y);
                be->setVelocity(vec::vec2(0.15f, 0.0f));
            } break;
                
            case BOTTOM:
            {
                be->position = vec::vec2(position.x, position.y - height);
                be->setVelocity(vec::vec2(0.0f, -0.15f));
            } break;
                
            case LEFT:
            {
                be->position = vec::vec2(position.x - width, position.y);
                be->setVelocity(vec::vec2(-0.15f, 0.0f));
            } break;
                
            default:
                break;
        }
    }
}

void Rectangle::shootABullet() { be->shootABullet(); }












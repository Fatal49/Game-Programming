
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
        vec::vec2(vertices[6], vertices[7]),    // Bottom Left
        vec::vec2(vertices[8], vertices[9])     // Bottom Right
    });
}

void Rectangle::debug() {
    debugFlag = true;
    std::vector<vec::vec2> myPoints = getPoints();
    vert.clear();
    i.clear();
    c.clear();
    
    vert.insert(vert.end(), {
        myPoints[0].x, myPoints[0].y,       // Top Left
        myPoints[1].x, myPoints[1].y,       // Top Right
        myPoints[2].x, myPoints[2].y,       // Bottom Left
        myPoints[3].x, myPoints[3].y,       // Bottom Right
    });
    
    i.insert(i.end(), {
        0,
        1,
        2,
        1,
        3,
        2
    });
    
    c.insert(c.end(), {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
    });
}

void Rectangle::draw(Shader* shader) {
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
    
    if(debugFlag) {
        Matrix m;
        shader->setModelMatrix(m);
        shader->bind();
        
        glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vert[0]);
        glEnableVertexAttribArray(shader->getPositionAttrib());
        
        glVertexAttribPointer(shader->getColorAttrib(), 4, GL_FLOAT, GL_FALSE, 0, &c[0]);
        glEnableVertexAttribArray(shader->getColorAttrib());
        
        glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_BYTE, &i[0]);
        
        glDisableVertexAttribArray(shader->getPositionAttrib());
        glDisableVertexAttribArray(shader->getColorAttrib());
        
        shader->unbind();
    }
}

void Rectangle::update(float elapsed) {
    translate(velocity.x * elapsed, velocity.y * elapsed);
}

void Rectangle::translate(float x, float y) {
    position.x = model.m[3][0];
    position.y = model.m[3][1];
    
    model.Translate(x, y, 0.0f);
}

void Rectangle::scale(float x, float y) {
    scaling.m[0][0] = x;
    scaling.m[1][1] = y;

    model.Scale(x, y, 1.0f);
}

void Rectangle::rotate(float angle) {
    rotating.m[0][0] = cos(angle);
    rotating.m[1][0] = -sin(angle);
    rotating.m[0][1] = sin(angle);
    rotating.m[1][1] = cos(angle);
    
    model.Rotate(angle);
}

void Rectangle::setVelocity(vec::vec2 v) { velocity = v; }

const std::vector<vec::vec2> Rectangle::getPoints() {
    std::vector<vec::vec2> p;
    Matrix temp;
    temp.m[0][3] = model.m[3][0];
    temp.m[1][3] = model.m[3][1];
    
    Matrix all = temp * scaling * rotating;
    
    for (int i = 0; i < points.size(); i++) {
        vec::vec4 v(points[i].x, points[i].y, 1.0f, 1.0f);
        vec::vec4 finalPos = all * v;
        p.push_back(vec::vec2(finalPos.x, finalPos.y));
    }
    
    return p;
}

bool Rectangle::checkCollision(const std::vector<vec::vec2> points) {
    const std::vector<vec::vec2> myPoints = getPoints();
    
    return testSATSeparationForEdge(myPoints[3].x - myPoints[1].x, myPoints[3].y - myPoints[1].y,
                                   myPoints, points);
}

const std::vector<vec::vec2> Rectangle::getEdges() {
    const std::vector<vec::vec2> p = getPoints();
    std::vector<vec::vec2> edges;
    
    edges.insert(edges.end(), {
        vec::vec2(p[1].x - p[0].x, p[1].y - p[0].y),        // Top edge
        vec::vec2(p[3].x - p[1].x, p[3].y - p[1].y),        // Right edge
        vec::vec2(p[2].x - p[3].x, p[2].y - p[3].y),        // Bottom edge
        vec::vec2(p[0].x - p[2].x, p[0].y - p[2].y)         // Left edge
    });
    
    return edges;
}

bool Rectangle::testSATSeparationForEdge(float edgeX, float edgeY,
                                          const std::vector<vec::vec2> &points1,
                                          const std::vector<vec::vec2> &points2) {
    float normalX = -edgeY;
    float normalY = edgeX;
    float len = sqrtf(normalX*normalX + normalY*normalY);
    normalX /= len;
    normalY /= len;
    
    std::vector<float> e1Projected;
    std::vector<float> e2Projected;
    
    for(int i=0; i < points1.size(); i++) {
        e1Projected.push_back(points1[i].x * normalX + points1[i].y * normalY);
    }
    for(int i=0; i < points2.size(); i++) {
        e2Projected.push_back(points2[i].x * normalX + points2[i].y * normalY);
    }
    
    std::sort(e1Projected.begin(), e1Projected.end());
    std::sort(e2Projected.begin(), e2Projected.end());
    
    float e1Min = e1Projected[0];
    float e1Max = e1Projected[e1Projected.size()-1];
    float e2Min = e2Projected[0];
    float e2Max = e2Projected[e2Projected.size()-1];
    float e1Width = fabs(e1Max-e1Min);
    float e2Width = fabs(e2Max-e2Min);
    float e1Center = e1Min + (e1Width/2.0);
    float e2Center = e2Min + (e2Width/2.0);
    float dist = fabs(e1Center-e2Center);
    float p = dist - ((e1Width+e2Width)/2.0);
    
    if(p < 0) {
        return true;
    }
    return false;
}

bool Rectangle::checkSATCollision(const std::vector<vec::vec2> &e1Points,
                                   const std::vector<vec::vec2> &e2Points) {
    for(int i=0; i < e1Points.size(); i++) {
        float edgeX, edgeY;
        
        if(i == e1Points.size()-1) {
            edgeX = e1Points[0].x - e1Points[i].x;
            edgeY = e1Points[0].y - e1Points[i].y;
        } else {
            edgeX = e1Points[i+1].x - e1Points[i].x;
            edgeY = e1Points[i+1].y - e1Points[i].y;
        }
        
        bool result = testSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points);
        if(!result) {
            return false;
        }
    }
    for(int i=0; i < e2Points.size(); i++) {
        float edgeX, edgeY;
        
        if(i == e2Points.size()-1) {
            edgeX = e2Points[0].x - e2Points[i].x;
            edgeY = e2Points[0].y - e2Points[i].y;
        } else {
            edgeX = e2Points[i+1].x - e2Points[i].x;
            edgeY = e2Points[i+1].y - e2Points[i].y;
        }
        bool result = testSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points);
        if(!result) {
            return false;
        }
    }
    return true;
}










#ifndef main_h
#define main_h

#include "includes/sdl_app.hpp"
#include "includes/Shader.hpp"
#include "includes/Entities.hpp"
#include "includes/gl_macros.hpp"
#include <ctime>

#define SHADERS "shaders/"
#define P_HEIGHT 1.0
#define P_WIDTH 0.5
#define PG_HEIGHT 0.5
#define PG_WIDTH 0.5

class inputParser {
public:
    
    eventParse operator() (SDL_Event event) {
        eventParse ep;
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                ep.exit = true;
                ep.sc = SDL_SCANCODE_ESCAPE;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                ep.up = true;
                ep.sc = SDL_SCANCODE_UP;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                ep.down = true;
                ep.sc = SDL_SCANCODE_DOWN;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                ep.up = true;
                ep.sc = SDL_SCANCODE_W;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S) {
                ep.down = true;
                ep.sc = SDL_SCANCODE_S;
            }
        }
        
        if (event.type == SDL_QUIT)
            ep.exit = true;
        
        return ep;
    }
    
};

class Game : public App {
private:

    // Shaders
    Shader* shader = NULL;
    
    // Vertices for objects
    std::vector<Vertex> v_paddle, v_pong;
    std::vector<GLubyte> i_paddle, i_pong;
    
    // Array object and buffer ID's
    GLuint arrayID, bufferID, indicesID;
    
    // Matrices
    Matrix model, view, projection;
    
    // Paddle and pong entities
    Entity left_paddle;
    Entity right_paddle;
    Entity pong;

    // Initialize the game
    void init() {
        
        // Vertices for paddle
        v_paddle.push_back(Vertex(vec::vec2(-0.5 * P_WIDTH, 0.5 * P_HEIGHT), vec::vec4(1, 1, 1, 1)));
        v_paddle.push_back(Vertex(vec::vec2(0.5 * P_WIDTH, 0.5 * P_HEIGHT), vec::vec4(1, 1, 1, 1)));
        v_paddle.push_back(Vertex(vec::vec2(-0.5 * P_WIDTH, -0.5 * P_HEIGHT), vec::vec4(1, 1, 1, 1)));
        v_paddle.push_back(Vertex(vec::vec2(0.5 * P_WIDTH, -0.5 * P_HEIGHT), vec::vec4(1, 1, 1, 1)));
        
        // Vertices for pong
        v_pong.push_back(Vertex(vec::vec2(-0.5 * PG_WIDTH, 0.5 * PG_HEIGHT), vec::vec4(1, 1, 1, 1)));
        v_pong.push_back(Vertex(vec::vec2(0.5 * PG_WIDTH, 0.5 * PG_HEIGHT), vec::vec4(1, 1, 1, 1)));
        v_pong.push_back(Vertex(vec::vec2(-0.5 * PG_WIDTH, -0.5 * PG_HEIGHT), vec::vec4(1, 1, 1, 1)));
        v_pong.push_back(Vertex(vec::vec2(0.5 * PG_WIDTH, -0.5 * PG_HEIGHT), vec::vec4(1, 1, 1, 1)));
        
        // Indices for paddle
        i_paddle.push_back(0);
        i_paddle.push_back(1);
        i_paddle.push_back(2);
        i_paddle.push_back(1);
        i_paddle.push_back(2);
        i_paddle.push_back(3);
        
        // Indices for square
        i_pong.push_back(4);
        i_pong.push_back(5);
        i_pong.push_back(6);
        i_pong.push_back(5);
        i_pong.push_back(6);
        i_pong.push_back(7);
        
        // Set vertices, indices, width and heigt for each entity
        left_paddle.setVectors(v_paddle, i_paddle);
        right_paddle.setVectors(v_paddle, i_paddle);
        pong.setVectors(v_pong, i_pong);
        left_paddle.setWidth(P_WIDTH);  left_paddle.setHeight(P_HEIGHT);
        right_paddle.setWidth(P_WIDTH);  right_paddle.setHeight(P_HEIGHT);
        pong.setWidth(PG_WIDTH);  pong.setHeight(PG_HEIGHT);
        
        // Set the orthographic projection
        projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
        
        // Load, compile & link the shaders
        shader = new Shader(SHADERS"vertex.glsl", SHADERS"fragment.glsl");
        
        // Create the VAO
        glGenVertexArrays(1, &arrayID);
        glBindVertexArray(arrayID);
        
        // Create the VBO
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, left_paddle.vertexSizeInBytes() + pong.vertexSizeInBytes(),
                     0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, left_paddle.vertexSizeInBytes(), &left_paddle.vertices[0]);
        glBufferSubData(GL_ARRAY_BUFFER, left_paddle.vertexSizeInBytes(), pong.vertexSizeInBytes(), &pong.vertices[0]);
        
        glGenBuffers(1, &indicesID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, left_paddle.indiceSizeInBytes() + pong.indiceSizeInBytes(), 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, left_paddle.indiceSizeInBytes(),
                        &left_paddle.indices[0]);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, left_paddle.indiceSizeInBytes(),
                        pong.indiceSizeInBytes(), &pong.indices[0]);
        
        // Enable Vertex Attributes
        glEnableVertexAttribArray(shader->getPositionAttrib());
        glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glEnableVertexAttribArray(shader->getColorAttrib());
        glVertexAttribPointer(shader->getColorAttrib(), 4, GL_FLOAT,
                              GL_FALSE, sizeof(Vertex), (void*)sizeof(vec::vec2));
        
        // Unbind the VAO
        glBindVertexArray(0);
        
        // Set the matrices
        shader->setModelMatrix(model);
        shader->setViewMatrix(view);
        shader->setProjectionMatrix(projection);
    }
    
    float distance(float x1, float y1, float x2, float y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }
    
    bool collisionWithBorder(float o_x, float o_y, float b_x, float b_y) {
        if (distance(o_x, o_y, b_x, b_y) == 0)
            return true;
        else
            return false;
    }
    
    float getAngle(point pos) {
        return 180 - atan2(pos.y, pos.x);
    }
    
    point randStart() {
        srand((unsigned)time(0));
        int neg_pos = rand() % 20 + 1;
        float x;
        if (neg_pos >= 10)
            x = -1;
        else
            x = 1;
        
        float top1 = float ((rand() % 10 + 1) / 5) * x;
        float top2 = float ((rand() % 10 + 1) / 5) * x;
        std::cout << "top1: " << top1 << ", top2: " << top2 << std::endl;
        
        point temp;
        temp.x = top1;
        temp.y = top2;
        temp.z = 1.0f;
        return temp;
    }
    
public:
    
    // Constructor
    Game(Uint32 flags, const char* name = "demo", int w = 1280, int h = 880)
    : App(flags, name, w, h) { init(); }
    
    // Destructor
    ~Game() {
        if (!shader)
            delete shader;
        glDeleteVertexArrays(1, &arrayID);
    }
    
    // Set up the objects
    void objectInit() {
        // left_paddle
        left_paddle.model.Translate(-3.45f, 0.0f, 0.0f);
        left_paddle.model.Scale(0.2f, 1.0f, 1.0f);
        left_paddle.setMass(2.0f);
        left_paddle.topLeft.x = left_paddle.vertices[0].position[0];
        left_paddle.topLeft.y = left_paddle.vertices[0].position[1];
        left_paddle.topRight.x = left_paddle.vertices[1].position[0];
        left_paddle.topRight.y = left_paddle.vertices[1].position[1];
        left_paddle.bottomLeft.x = left_paddle.vertices[2].position[0];
        left_paddle.bottomLeft.y = left_paddle.vertices[2].position[1];
        left_paddle.bottomRight.x = left_paddle.vertices[3].position[0];
        left_paddle.bottomRight.y = left_paddle.vertices[3].position[1];

        // right_paddle
        right_paddle.model.Translate(3.45f, 0.0f, 0.0f);
        right_paddle.model.Scale(0.2f, 1.0f, 1.0f);
        right_paddle.setMass(2.0f);
        right_paddle.topLeft.x = right_paddle.vertices[0].position[0];
        right_paddle.topLeft.y = right_paddle.vertices[0].position[1];
        right_paddle.topRight.x = right_paddle.vertices[1].position[0];
        right_paddle.topRight.y = right_paddle.vertices[1].position[1];
        right_paddle.bottomLeft.x = right_paddle.vertices[2].position[0];
        right_paddle.bottomLeft.y = right_paddle.vertices[2].position[1];
        right_paddle.bottomRight.x = right_paddle.vertices[3].position[0];
        right_paddle.bottomRight.y = right_paddle.vertices[3].position[1];
        
        // Pong (square)
        pong.xVel = 0.3f;
        pong.yVel = 0.3f;
        point temp = randStart();
        if (temp.x > 2.0f || temp.y < -2.0f)
            temp = randStart();
        if (temp.x < -2.0f || temp.y < -2.0f)
            temp = randStart();
        pong.model.Translate(temp.x, temp.y, temp.z);
        pong.model.Scale(0.25f, 0.25f, 1.0f);
        pong.setMass(2.0f);
        pong.vector.angle = getAngle(temp);
        pong.topLeft.x = pong.vertices[0].position[0];
        pong.topLeft.y = pong.vertices[0].position[1];
        pong.topRight.x = pong.vertices[1].position[0];
        pong.topRight.y = pong.vertices[1].position[1];
        pong.bottomLeft.x = pong.vertices[2].position[0];
        pong.bottomLeft.y = pong.vertices[2].position[1];
        pong.bottomRight.x = pong.vertices[3].position[0];
        pong.bottomRight.y = pong.vertices[3].position[1];
    }
    
    void onDraw(eventParse ep, float elapsed) {
        shader->bind();
        glBindVertexArray(arrayID);
        float x_neg_pos = 1.0f, y_neg_pos = 1.0f;
        
        // left paddle
        if (ep.up && ep.sc == SDL_SCANCODE_UP) {
            if (left_paddle.topLeft.y + left_paddle.model.m[3][1] < 2.0f) {
                left_paddle.model.Translate(0.0f, 0.05f, 0.0f);
            }
        } else if (ep.down && ep.sc == SDL_SCANCODE_DOWN) {
            if (left_paddle.bottomLeft.y + left_paddle.model.m[3][1] > -2.0f) {
                left_paddle.model.Translate(0.0f, -0.05f, 0.0f);
            }
        }
        shader->setModelMatrix(left_paddle.getModelMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
        
        
        // right paddle
        if (ep.up && ep.sc == SDL_SCANCODE_W) {
            if (right_paddle.topLeft.y + right_paddle.model.m[3][1] < 2.0f) {
                right_paddle.model.Translate(0.0f, 0.05f, 0.0f);
            }
        } else if (ep.down && ep.sc == SDL_SCANCODE_S) {
            if (right_paddle.bottomLeft.y + right_paddle.model.m[3][1] > -2.0f) {
                right_paddle.model.Translate(0.0f, -0.05f, 0.0f);
            }
        }
        shader->setModelMatrix(right_paddle.getModelMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
        
        
        // pong
        float top = pong.topRight.y + pong.model.m[3][1];
        float bottom = pong.bottomRight.y + pong.model.m[3][1];
        if (top >= 1.95f && top <= 2.05f) {
            pong.model.Translate(0.0f, -0.09f, 0.0f);
        }
        if (bottom >= -2.05f && bottom  <= -1.95f) {
            pong.model.Translate(0.0f, 0.09, 0.0f);
        }
        std::cout << "(" << pong.topRight.x + pong.model.m[3][0] <<  ", "
            << pong.topRight.y + pong.model.m[3][1] << ")" << std::endl;
        pong.model.m[3][0] += elapsed * cos(pong.vector.angle);
        pong.model.m[3][1] += elapsed * sin(pong.vector.angle);
        shader->setModelMatrix(pong.getModelMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)6);
        

        glBindVertexArray(0);
        shader->unbind();
    }
    
};



#endif /* main_h */

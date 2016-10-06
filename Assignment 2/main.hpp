#ifndef __MAIN_H__
#define __MAIN_H__

#include "../includes/sdl_app.hpp"
#include "../includes/sdl_window.hpp"
#include "../includes/gl_macros.hpp"
#include "../includes/Shader.hpp"
#include "../includes/sdl_events.hpp"
#include "../includes/Entities.hpp"

#define SHADERS "./shaders/"
const float heightRatio = 1.0f;
const float widthRatio = 16.0f / 9.0f;

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

enum dir {UP, DOWN, LEFT, RIGHT};

struct _vec {
    dir direction;
    float xVel = 0.0f;
    float yVel = 0.0f; 
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
        v_paddle.push_back(Vertex( vec::vec2(-0.5f, 0.5f), vec::vec4(1, 1, 1, 1) ));
        v_paddle.push_back(Vertex( vec::vec2(0.5f, 0.5f), vec::vec4(1, 1, 1, 1) ));
        v_paddle.push_back(Vertex( vec::vec2(-0.5f, -0.5f), vec::vec4(1, 1, 1, 1) ));
        v_paddle.push_back(Vertex( vec::vec2(0.5f, -0.5f), vec::vec4(1, 1, 1, 1) ));

        // Verrices for square
        v_pong.push_back(Vertex( vec::vec2(-0.25f, 0.25f), vec::vec4(1, 1, 1, 1) ));
        v_pong.push_back(Vertex( vec::vec2(0.25f, 0.25f), vec::vec4(1, 1, 1, 1) ));
        v_pong.push_back(Vertex( vec::vec2(-0.25f, -0.25f), vec::vec4(1, 1, 1, 1) ));
        v_pong.push_back(Vertex( vec::vec2(0.25f, -0.25f), vec::vec4(1, 1, 1, 1) ));

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
        left_paddle.setWidth(1.0f);  left_paddle.setHeight(1.0f);
        right_paddle.setWidth(1.0f);  right_paddle.setHeight(1.0f);
        pong.setWidth(0.5f);  pong.setHeight(0.5f);

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
        glBufferSubData(GL_ARRAY_BUFFER, 0, left_paddle.vertexSizeInBytes(), left_paddle.getVertexData());
        glBufferSubData(GL_ARRAY_BUFFER, left_paddle.vertexSizeInBytes(), pong.vertexSizeInBytes(), 
                pong.getVertexData());

        glGenBuffers(1, &indicesID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, left_paddle.indiceSizeInBytes() + pong.indiceSizeInBytes(), 
                0, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, left_paddle.indiceSizeInBytes(), 
                left_paddle.getIndicesData());
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, left_paddle.indiceSizeInBytes(), 
                pong.indiceSizeInBytes(), pong.getIndicesData());

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

    bool collision(float x1, float y1, float radius1, float x2, float y2) {
        if (distance(x1, y1, x2, y2) < radius1)
            return true;
        else
            return false;
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

    void objectInit() {
        
        // left_paddle
        left_paddle.model.setPosition(-3.45f, 0.0f, 0.0f);
        left_paddle.model.setScale(0.1f, 1.0f, 1.0f);

        // right_paddle
        right_paddle.model.Translate(3.45f, 0.0f, 0.0f);
        right_paddle.model.setScale(0.1f, 1.0f, 1.0f);

        // Pong (square)
        pong.model.setScale(0.25f, 0.25f, 1.0f);
    }

    void onDraw(eventParse ep, float elapsed) {

        // Use the compiled shaders
        shader->bind(); 

        // Bind VAO
        glBindVertexArray(arrayID);

        // Draw the left paddle
        if (ep.up && ep.sc == SDL_SCANCODE_UP) {
            if (distance(left_paddle.model.m[3][0], left_paddle.model.m[3][1], left_paddle.model.m[3][0], 2.0f)
                    > (0.5f * left_paddle.getHeight()))
                left_paddle.model.Translate(0.0f, 0.05f, 0.0f);
        }
        if (ep.down && ep.sc == SDL_SCANCODE_DOWN) {
            if (distance(left_paddle.model.m[3][0], left_paddle.model.m[3][1], left_paddle.model.m[3][0], -2.0f)
                    > (0.5f * left_paddle.getHeight()))
            left_paddle.model.Translate(0.0f, -0.05f, 0.0f);
        } 
        shader->setModelMatrix(left_paddle.getModelMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);


        // Draw the right paddle
        if (ep.up && ep.sc == SDL_SCANCODE_W) {
            if (distance(right_paddle.model.m[3][0], right_paddle.model.m[3][1], right_paddle.model.m[3][0], 2.0f)
                    > (0.5f * right_paddle.getHeight()))
            right_paddle.model.Translate(0.0f, 0.05f, 0.0f);
        }
        if (ep.down && ep.sc == SDL_SCANCODE_S) {
            if (distance(right_paddle.model.m[3][0], right_paddle.model.m[3][1], 
                        right_paddle.model.m[3][0], -2.0f)
                    > (0.5f * right_paddle.getHeight()))
            right_paddle.model.Translate(0.0f, -0.05f, 0.0f);
        } 
        shader->setModelMatrix(right_paddle.getModelMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);


        // Draw the pong (square) 
        if (collision(right_paddle.getX(), right_paddle.getY(), right_paddle.getWidth() / 5.0, 
                    pong.getX(), pong.getY())) {
            pong.model.Translate(-1.0f * elapsed, 0.0f, 0.0f);
        } else {
            pong.model.Translate(elapsed, 0.0f, 0.0f);
        }
        shader->setModelMatrix(pong.getModelMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)6);
        
        // Unbind VAO
        glBindVertexArray(0);    

        // Remove the shaders
        shader->unbind();
    } 
};


#endif /* __MAIN_H__ defined */

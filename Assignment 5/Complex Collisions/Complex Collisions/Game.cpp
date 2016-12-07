#include "Game.hpp"

Game::Game(float width, float height, const char* name)
    : height(height), width(width), name(name) {}

Game::Game(const char* name) : height(800), width(1280), name(name) {}

Game::~Game() {
    if (window) {
        printf("Destroying SDL_Window\n");
        SDL_DestroyWindow(window);
    }
}

void Game::gameLoop() {
    if (window) {
        setup();
        while(processEvents()) {
            update();            
            render();
        }
    } else {
        printf("Call createWindow() before using this function.\n");
    }
}

bool Game::createWindow() {
    // Create the window
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_OPENGL);
    
    // Check if the window was created successfully
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());        
        return false;
    }
    
    // Create a handle to the OpenGL context
    context = SDL_GL_CreateContext(window);
    
    // Setup the OpenGL context for rendering
    SDL_GL_MakeCurrent(window, context);
    
    // Return success
    return true;
}

void Game::setViewPort() {
    if (window)
        glViewport(0, 0, width, height);
    else
        printf("Call createWindow() before using this function.\n");
}

void Game::swapBuffers() {
    if (window)
        SDL_GL_SwapWindow(window);
    else
        printf("Call createWindow() before using this function.\n");
}

float Game::getWidth() const { return width; }

float Game::getHeight() const { return height; }



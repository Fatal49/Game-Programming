
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#include "../includes/ShaderProgram.hpp"
#include "../includes/Matrix.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800
#define SHADER "./shaders/"

int main(int argc, char* argv[]) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Initialization Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create the window
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);

    std::vector<float> positions = {-0.5f, 0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

    ShaderProgram sp = ShaderProgram(SHADER"vertex.glsl", SHADER"fragment.glsl");
    Matrix model;
    Matrix view;
    Matrix projection;
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

    bool exit = false;
    SDL_Event event;

    // Main game loop
    while (!exit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit = true;
        }

        // Set the color for the viewport
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sp.setModelMatrix(model);
        sp.setViewMatrix(view);
        sp.setProjectionMatrix(projection);

        glVertexAttribPointer(sp.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, &positions[0]);

            // Draw the triangles
            glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(sp.positionAttribute);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

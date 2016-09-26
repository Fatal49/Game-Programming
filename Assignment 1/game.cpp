
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

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
        GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void drawSprite(GLuint sprite, GLuint position, GLuint texCooord,
    const float* pos, const float* texture, int triangles) {
        glBindTexture(GL_TEXTURE_2D, sprite);
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, pos);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(texCooord, 2, GL_FLOAT, GL_FALSE, 0, texture);
        glEnableVertexAttribArray(texCooord);

            glDrawArrays(GL_TRIANGLES, 0, triangles);

        glDisableVertexAttribArray(position);
        glDisableVertexAttribArray(texCooord);
}

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

    // Create and set context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::vector<float> positions = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    std::vector<float> texCoords = {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    Matrix model;
    Matrix view;
    Matrix projection;
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

    ShaderProgram sp = ShaderProgram(SHADER"vertex_textured.glsl", SHADER"fragment_textured.glsl");
    GLuint sprite1 = LoadTexture("./images/sprite1.png");
    GLuint sprite2 = LoadTexture("./images/sprite2.png");
    GLuint sprite3 = LoadTexture("./images/sprite3.png");

    float lastFrameticks = 0.0f, angle = 0.0f;
    bool exit = false;
    SDL_Event event;

    // Main game loop
    while (!exit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit = true;
        }

        // For annimation
        float ticks = (float) SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - lastFrameticks;
        lastFrameticks = ticks;
        angle += elapsed;

        // Set the color for the viewport
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sprite 1
        model.identity();
        model.Translate(-2.2f, 0.0f, 0.0f);
        model.Scale(1.0f, 1.0f, 1.0f);
        sp.setModelMatrix(model);
        sp.setViewMatrix(view);
        sp.setProjectionMatrix(projection);
        drawSprite(sprite1, sp.positionAttribute, sp.texCoordAttribute, &positions[0], &texCoords[0], 6);


        // Sprite 2
        model.identity();
        model.Translate(2.2f, 0.0f, 0.0f);
        sp.setModelMatrix(model);
        sp.setViewMatrix(view);
        sp.setProjectionMatrix(projection);
        drawSprite(sprite2, sp.positionAttribute, sp.texCoordAttribute, &positions[0], &texCoords[0], 6);


        // Sprite 3
        model.identity();
        model.Rotate(angle);
        sp.setModelMatrix(model);
        sp.setViewMatrix(view);
        sp.setProjectionMatrix(projection);
        drawSprite(sprite3, sp.positionAttribute, sp.texCoordAttribute, &positions[0], &texCoords[0], 6);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

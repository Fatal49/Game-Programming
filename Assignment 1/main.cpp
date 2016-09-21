#include "../includes/sdl_window.hpp"
#include "../includes/sdl_app.hpp"

using namespace graphics;

int main(int argc, char* argv[]) {

    // // Initialize SDL
    // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    //     printf("SDL failed to initialize, Error: %s\n", SDL_GetError());
    //     exit(EXIT_FAILURE);
    // }
    //
    // // Create the window
    // Window* window = new Window();
    // window->create(1280, 800, "Demo");
    //
    // // Set the viewport
    // window->setViewPort();
    //
    // // The main loop
    // SDL_Event event;
    // bool exit = false;
    //
    // while(!exit) {
    //     // Get the next event from the queue
    //     while(SDL_PollEvent(&event)) {
    //         if (event.type == SDL_QUIT)
    //             exit = true;
    //
    //
    //     }
    //
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     window->swapBuffers();
    // }
    //
    // // Cleanup
    // delete window;
    // SDL_Quit();

    App a = App(SDL_INIT_EVERYTHING, "Game Demo");

    a.start();

    return 0;
}

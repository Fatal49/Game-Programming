#include "window.h"

window::window(const char* name, int w, int h) : width(w), height(h), windowName(name) {}
window::window(const window& rhs)
    : width(rhs.getWidth()), height(rhs.getHeight()), windowName(rhs.getWindowName()) {}
window::~window() {}

SDL_Window* window::getSDLwindow() const { return current; }
int window::getWidth() const { return width; }
int window::getHeight() const  { return height; }
const char* window::getWindowName() const { return windowName; }

void window::setViewPort() { glViewport(0, 0, width, height); }

void window::create() {
    current = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    
    // Error check
    if (!current) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    context = SDL_GL_CreateContext(current);
    SDL_GL_MakeCurrent(current, context);
}

void window::destroyWindow() { SDL_DestroyWindow(current); }
void window::swapBuffers() { SDL_GL_SwapWindow(current); }
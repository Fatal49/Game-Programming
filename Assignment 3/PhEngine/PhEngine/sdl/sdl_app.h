#ifndef __SDL_APP_HPP__
#define __SDL_APP_HPP__

#include <SDL2/SDL.h>
    #define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include "window.h"
#include "sdl_events.h"

// 
// -- Wrapper for the main game loop --
//
class App {
private:

    // Window
    Window mWindow;

public:

    // Constructor
    App(Uint32 flags, const char* name = "demo", int w = 1280, int h = 800) {
        // Initialize SDL
        if ( SDL_Init(flags) != 0 ) {
            printf("Initialization Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        // Create the window
        mWindow.create(w, h, name);
        printf("SDL window is created\n");

        // Set the viewport
        mWindow.setViewPort();

        // Enable alpha blending & depth
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    // Deconstructor
    ~App() { printf("Quiting SDL\n"); SDL_Quit(); }
    
    // Getter
    inline Window& window() { return mWindow; }
    
    // Start the main loop of the game
    template <class Object>
    void start(Object eventHandler) {
        printf("Starting ...\n");
        float lastFrameticks = 0.0f;
        float angle = 0.0f;
        bool exit = false;
        Event<Object> h(&eventHandler);
        eventParse ep;

        // Initialize all object positions
        objectInit();

        // Main game loop
        while (!exit) {
            while (h.getEvent()) {
                ep = h.handleEvent();
                if (ep.exit)
                    exit = true;
            }
            
            // For annimation
            float ticks = (float) SDL_GetTicks() / 1000.0f;
            float elapsed = ticks - lastFrameticks;
            lastFrameticks = ticks;
            angle += elapsed;

            // Set the color for the viewport
            // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            
            // Clear the buffer with preset values
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Call overloaded draw function
            onDraw(ep, elapsed, angle);

            // Swap buffers
            mWindow.swapBuffers();
        }
    }

    // Virtual functions
    virtual void onDraw(const eventParse& ep, float elapsed, float angle) {}
    virtual void objectInit() {}

};

#endif /* __SDL_APP_HPP__ defined */

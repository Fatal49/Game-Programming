//////////////////////////////////////////////////////////////////////////////
//
//  --- sdl_app.hpp ---
//
//   Header for the app class
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __SDL_APP_HPP__
#define __SDL_APP_HPP__

//----------------------------------------------------------------------------
//
// --- Include system headers ---
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include "sdl_window.hpp"

namespace graphics {

    /*-----------------------------------------------------------------------------
     * Wrapper for main game loop
     *-----------------------------------------------------------------------------*/
     class App {
     private:

         // Member variables
         Window mWindow;

     public:

         // Getter
         inline Window& window() { return mWindow; }

         // Deconstructor
         ~App() { printf("Quiting SDL"); SDL_Quit(); }

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
             glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
         }

         // Start the main loop of the game
         void start() {
             printf("Starting ...\n");
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

                // Clear the buffer with preset values
                 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                 // Call overloaded draw function
                 onDraw();

                 // Swap buffers
                 mWindow.swapBuffers();
             }
         }

         // Overload functions
         virtual void onDraw() {}

     };    // class App

}   // namespace graphics


#endif /* __SDL_APP_HPP__ defined */

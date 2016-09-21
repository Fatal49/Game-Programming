//////////////////////////////////////////////////////////////////////////////
//
//  --- sdl_window.hpp ---
//
//   The main header for the SDL window
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __SDL_WINDOW_HPP__
#define __SDL_WINDOW_HPP__

//----------------------------------------------------------------------------
//
// --- Include system headers ---
//

 #include <SDL2/SDL.h>
 #include <SDL2/SDL_opengl.h>
 #include <stdio.h>

 namespace graphics {

     /*-----------------------------------------------------------------------------
      *  A SDL Window Wrapper
      *-----------------------------------------------------------------------------*/
      class Window {
      private:

          // Member variables
          SDL_Window* window;
          SDL_GLContext context;
          int mWidth, mHeight;

      public:

          // Getters
          inline int width() { return mWidth; }
          inline int height() { return mHeight; }
          inline float ratio() { return (float) mWidth / mHeight; }
          SDL_Window*& getSDLWindow() { return window; }

          // Default Constructor
          Window() {}

          // Create a window context
          void create(int w, int h, const char* name = "demo") {
              // Set the width and height
              mWidth = w; mHeight = h;

              // Create the window
              window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);

              // Check if the window was created successfully
              if (!window) {
                  printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
                  SDL_Quit();
                  exit(EXIT_FAILURE);
              }

              // Create a handle to the OpenGL context
              context = SDL_GL_CreateContext(window);

              // Setup the OpenGL context for rendering
              SDL_GL_MakeCurrent(window, context);
          }

          // Set the viewport
          void setViewPort() { glViewport(0, 0, mWidth, mHeight); }

          // Use double buffering
          void swapBuffers() { SDL_GL_SwapWindow(window); }

          // Destructor
          ~Window() { SDL_DestroyWindow(window); }

      }; // Window class

 } // namespace graphics

#endif /* __SDL_WINDOW_HPP__ defined */

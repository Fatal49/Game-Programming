//////////////////////////////////////////////////////////////////////////////
//
//  --- sdl_events.hpp ---
//
//   Events class to handle all events sent o SDL
//
//////////////////////////////////////////////////////////////////////////////

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
     * Wrapper for SDL Events
     *-----------------------------------------------------------------------------*/
     class Event {
     private:

         // Member variables
         SDL_Event current;

     public:

         // Default Constructor
         Event() {}

         // Attempt to get the next event in the queue
         int getEvent() { return SDL_PollEvent(&current); }

         // Handle the event
         void handleEvent() {             


         }



     };   // class Events

}   // namespace graphics

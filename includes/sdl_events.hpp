#ifndef __SDL_EVENTS_CPP__
#define __SDL_EVENTS_CPP__

#include "sdl_window.hpp"

typedef struct {
    bool exit = false;
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
    bool space = false;
    SDL_Scancode sc;
} eventParse;

//
// -- Wrapper for SDL Events --
//
template<typename Object> class Event {
private:

    // Pointer to an object
    Object* __ptr2Object;

    // The next event to be handled
    SDL_Event event;

public:

    // Constructor
    Event(Object* ptr2Object) :__ptr2Object(ptr2Object) {}

    // Attempt to get the next event in the queue
    int getEvent() { return SDL_PollEvent(&event); }

    // Handle the next event
    // Any functor passed to handleEvent must have a
    // () operator overload taking a SDL_Event structure
    // as its argument
    eventParse handleEvent() {
        return (*__ptr2Object)(event);
    }

};

#endif /* __SDL_EVENTS_CPP__ defiend */

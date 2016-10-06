#include "sdl_events.hpp"

template <typename Object>
Event::Event(Object* ptr2Object) :__ptr2Object(ptr2Object) {}

int Event::getEvent() { return SDL_PollEvent(&event); }

int Event::handleEvent() {
    return (*__ptr2Object)(event);
}

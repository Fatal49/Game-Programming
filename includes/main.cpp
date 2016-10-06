#include "sdl_events.hpp"
#include <iostream>

using namespace std;

class handler {
public:
    int operator() (SDL_Event event) const {
        return 1; 
    }
};

int main() {

    handler h;
    Event<handler> e(&h);
    cout << e.handleEvent() << endl;

    return 0;
}

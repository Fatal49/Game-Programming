#include "Game.h"

class inputParser {
public:
    
    eventParse operator() (SDL_Event& event) {
        eventParse ep;
        
        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                ep.exit = true;
                ep.sc = SDL_SCANCODE_ESCAPE;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                ep.right = true;
                ep.sc = SDL_SCANCODE_RIGHT;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                ep.left = true;
                ep.sc = SDL_SCANCODE_LEFT;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                ep.space = true;
                ep.sc = SDL_SCANCODE_SPACE;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                ep.sc = SDL_SCANCODE_RETURN;
            }
        } else if (event.type == SDL_QUIT)
            ep.exit = true;
        
        return ep;
    }
    
};

int main(int argc, const char * argv[]) {

    inputParser ip;
    Game myGame(SDL_INIT_EVERYTHING, "Space Invaders", 1280, 800);
    myGame.start(ip);
    
    return 0;
}

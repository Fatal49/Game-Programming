#include "main.h"

int main(int argc, const char * argv[]) {

    inputParser ip;
    Game myGame(SDL_INIT_EVERYTHING, "Ping Pong", 1280, 800);
    myGame.start(ip);
    
    return 0;
}

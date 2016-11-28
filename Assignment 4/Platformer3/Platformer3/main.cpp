#include "Platformer.hpp"

int main(int argc, const char * argv[]) {

    Platformer game = Platformer(1024, 576, "Platformer");
    game.gameLoop();
    
    return 0;
}

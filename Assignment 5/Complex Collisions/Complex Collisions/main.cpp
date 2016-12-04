
#include "Platformer.hpp"

int main(int argc, const char * argv[]) {
    
    Platformer game = Platformer(1024, 576, "Complex Collisions");
    game.gameLoop();
    
    return 0;
}

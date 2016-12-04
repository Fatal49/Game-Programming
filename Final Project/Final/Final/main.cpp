#include "Platformer.hpp"

int main(int argc, const char * argv[]) {

    Platformer final = Platformer(1024, 576, "Final Project");
    final.gameLoop();
    
    return 0;
}

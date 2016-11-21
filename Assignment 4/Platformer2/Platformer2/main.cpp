 #include "Platformer.hpp"

int main(int argc, const char * argv[]) {
    
    // 1280, 720
    // 1024, 576
    
    // Create Platformer and start the game
    Platformer mario = Platformer(1280, 720, "Platformer");
    mario.gameLoop();

    return 0;
}

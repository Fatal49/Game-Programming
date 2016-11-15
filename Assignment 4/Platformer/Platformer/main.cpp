 #include "Platformer.hpp"

int main(int argc, const char * argv[]) {
    
    // Create Platformer and start the game
    Platformer mario = Platformer(1280, 720, "Mario");
    mario.gameLoop();

    return 0;
}

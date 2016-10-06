#include "main.hpp"

using namespace std;
using namespace vec;

int main(int argc, char* argv[]) {

    inputParser ip;
    Game game(SDL_INIT_EVERYTHING, "Ping Pong", 1280, 800);
    game.start(ip);

    return 0;
}

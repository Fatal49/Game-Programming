#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Game {
public:
    
    // Constructors & Deconstructor
    Game(float width, float height, const char* name);
    Game(const char* name);
    ~Game();
    
    // Methods
    void gameLoop();
    bool createWindow();
    void setViewPort();
    void swapBuffers();
    float getWidth() const;
    float getHeight() const;
    void setPlayingFlag(bool p);
    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    
private:
    // SDL window
    SDL_Window* window;
    
    // SDL context
    SDL_GLContext context;
    
    // Window height and width
    float height, width;
    
    // Window name
    const char* name;
    
    // Game flag
    bool playing;
};



#endif /* __GAME_HPP__ defined */

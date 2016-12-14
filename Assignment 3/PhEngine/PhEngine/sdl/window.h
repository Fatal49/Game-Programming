#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

class window {
private:
    
    int width, height;
    SDL_Window* current;
    SDL_GLContext context;
    const char* windowName;
    
public:
    
    window(const char* w_name, int w, int h);
    window(const window& rhs);
    ~window();
    
    SDL_Window* getSDLwindow() const;
    int getWidth() const;
    int getHeight() const;
    const char* getWindowName() const;
    
    void setViewPort();
    
    void create();
    void destroyWindow();
    void swapBuffers();
    
    SDL_Event e;
    
};


#endif /* __WINDOW_H__ defined */
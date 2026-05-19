#pragma once


#include <SDL3/SDL.h>

class Window {
public:
    Window();
    ~Window();
    bool initialise();
    SDL_Window = get_window();
private:
	SDL_Window* window; 
};
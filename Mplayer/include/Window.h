#pragma once


#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>


class Window {
public:
    Window();
    ~Window();
    bool initialise(SDL_Window*& window,
        SDL_Renderer*& renderer);
    SDL_Window = get_window();
private
};
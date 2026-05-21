#pragma once

#include <SDL3/SDL.h>
#include "./FFmpegDecoder.h"


class Window {

public:

    Window();
    ~Window();

    bool initialise(int w,int h);

    bool processEvents();

    SDL_Window* get_window();

private:

    SDL_Window* window;
};
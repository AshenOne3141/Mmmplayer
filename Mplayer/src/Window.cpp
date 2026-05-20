#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>
#include <iostream>
#include "../include/Window.h"
Window::Window() 
	: window(nullptr) {}
    
Window::~Window() {
    if(window)SDL_DestroyWindow(window);
    SDL_Quit();
}
bool Window::initialise() {

    if (!SDL_Init(SDL_INIT_VIDEO) ) {
        std::cout << "INITIALISATION ERROR "
            << SDL_GetError();
        return false;
    }

    window = SDL_CreateWindow("Interface", 800, 800, SDL_WINDOW_OPENGL);

    if (!window)
    {    std::cout << SDL_GetError() << '\n';
        return false;
    }

   

    return true;
}

bool Window::processEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_EVENT_QUIT)
            return false;

        if (event.type == SDL_EVENT_KEY_DOWN &&
            event.key.key == SDLK_ESCAPE)
            return false;
    }

    return true;
}

SDL_Window* Window::get_window() {
    return window;
}
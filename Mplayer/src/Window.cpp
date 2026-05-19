#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>
#include <iostream>
#include "../include/Window.h"
bool initialise(SDL_Window*& window, SDL_Renderer*& renderer) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "INITIALISATION ERROR "
            << SDL_GetError();
        return false;
    }

    window = SDL_CreateWindow("Interface", 800, 800, SDL_WINDOW_OPENGL);

    if (window == NULL)
        return false;

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL)
        return false;

    return true;
}

void close_window(SDL_Window* window, SDL_Renderer* renderer) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

SDL_Texture* load_media(std::string path, SDL_Renderer* renderer) {

    SDL_Surface* img = IMG_Load(path.c_str());

    if (img == NULL) {

        std::cout << "ERROR INIT-ING THE SURFACE";

        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, img);

    SDL_DestroySurface(img);

    return texture;
}
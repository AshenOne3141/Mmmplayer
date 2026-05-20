#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "../include/Renderer.h"
#include "../include/Window.h"

Renderer::Renderer(Window& win)
    : window(win),
    renderer(nullptr),
    texture(nullptr)
{
}
Renderer::~Renderer() {
    if (texture)
        SDL_DestroyTexture(texture);

    if (renderer)
        SDL_DestroyRenderer(renderer);
}
bool Renderer::initialise() {
    
	renderer = SDL_CreateRenderer(window.get_window(),nullptr);
    if (!renderer) {
        std::cout << "Renderer not initialised\n";
        return false;
    }
    SDL_Surface* surface = IMG_Load("C:\\Users\\jaisw\\OneDrive\\Pictures\\images.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) {

        std::cout << "Texture creation failed\n";

        return false;
    }

    return true;
   	

      
}
void Renderer::render() {
    SDL_RenderClear(renderer);

    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        nullptr
    );

    SDL_RenderPresent(renderer);
}

	
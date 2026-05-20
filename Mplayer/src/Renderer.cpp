#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "../include/Renderer.h"
#include "../include/Window.h"
#include <vector>
Renderer::Renderer(Window& win)
    : window(win),
    renderer(nullptr),
    texture(nullptr),
    frameNumber(0),
    paused(false),
    speed(1)

   
{
    pixels.resize(800 * 800);
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

    for (int y = 0; y < 800; y++) {

        for (int x = 0; x < 800; x++) {

            uint8_t r = (x + frameNumber) % 256;
            uint8_t g = (y + frameNumber) % 256;
            uint8_t b = (x + y + frameNumber) % 256;

            pixels[y * 800 + x] =
                (255 << 24) |
                (r << 16) |
                (g << 8) |
                b;
        }
    }

    SDL_UpdateTexture(
        texture,
        nullptr,
        pixels.data(),
        800 * sizeof(uint32_t)
    );

    SDL_RenderClear(renderer);

    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        nullptr
    );

    SDL_RenderPresent(renderer);

    if(!paused)frameNumber+=speed;
}
void Renderer::togglePause() {
    paused = !paused;
}

void Renderer::increaseSpeed() {
    speed++;
}

void Renderer::decreaseSpeed() {

    if (speed > 1)
        speed--;
}
   


	
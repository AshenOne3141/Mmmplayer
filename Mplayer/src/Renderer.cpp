#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "../include/Renderer.h"
#include "../include/Window.h"
#include <vector>
#include "../include/FFmpegDecoder.h"
Renderer::Renderer(Window& win)
    : window(win),
    renderer(nullptr),
    texture(nullptr),
    frameNumber(0),
    paused(false),
    speed(1) {
}

   

Renderer::~Renderer() {
    if (texture)
        SDL_DestroyTexture(texture);

    if (renderer)
        SDL_DestroyRenderer(renderer);
}
bool Renderer::initialise(int w,int h) {
    
	renderer = SDL_CreateRenderer(window.get_window(),nullptr);
    if (!renderer) {
        std::cout << "Renderer not initialised\n";
        return false;
    }
   
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STREAMING,w,h);
    
    if (!texture) {

        std::cout << "Texture creation failed\n";

        return false;
    }

    return true;
   	

      
}
void Renderer::render(AVFrame* rgbFrame) {

    SDL_UpdateTexture(
        texture,
        nullptr,
       rgbFrame->data[0],
       rgbFrame->linesize[0]
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

	
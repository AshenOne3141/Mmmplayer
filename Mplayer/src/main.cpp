#include <SDL3/SDL.h>
#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "../include/Window.h"
int main(int argc, char* argv[]) {

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initialise(window, renderer))
        return -1;

    SDL_Texture* texture =
        load_media("C:\\Users\\jaisw\\OneDrive\\Pictures\\images.jpg", renderer);

    bool quit = false;

    SDL_Event event;

    while (!quit) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                quit = true;
        }

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer,
            texture,
            NULL,
            NULL);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);

    close_window(window, renderer);

    return 0;
}
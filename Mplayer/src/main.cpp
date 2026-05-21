#include "../include/Window.h"
#include "../include/Renderer.h"
#include "../include/FFmpegDecoder.h"

int main(int argc, char* argv[]) {

    FFmpegDecoder decoder;

    if (!decoder.openFile("C:\\Users\\jaisw\\Downloads\\The Boys S05E07 The Frenchman the Female and the Man Called Mother's Milk 720p AMZN WEB-DL DD 5 1 Atmos H 264-playWEB[EZTVx.to].mkv"))
        return -1;

    Window window;

    if (!window.initialise(
        decoder.getWidth(),
        decoder.getHeight()))
    {
        return -1;
    }

    Renderer renderer(window);

    if (!renderer.initialise(
        decoder.getWidth(),
        decoder.getHeight()))
    {
        return -1;
    }

    bool running = true;

    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {

                switch (event.key.key) {

               

                case SDLK_SPACE:
                    decoder.togglePause();
                    break;
                case SDLK_LEFT:
                    decoder.rewind();
                    break;
                case SDLK_RIGHT:
                    decoder.forward();
                    break;

               
                }
            }
        }

        if (decoder.decodeFrame()) {

            renderer.render(
                decoder.getRGBFrame()
            );
        }

        SDL_Delay(1000/decoder.getfps());
    }

    return 0;
}
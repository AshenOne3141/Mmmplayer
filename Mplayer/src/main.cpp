#include "../include/Window.h"
#include "../include/Renderer.h"
#include "../include/FFmpegDecoder.h"

int main(int argc, char* argv[]) {

    FFmpegDecoder decoder;

    if (!decoder.openFile("C:\\Users\\jaisw\\Downloads\\file_example_MP4_1920_18MG.mp4"))
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

                case SDLK_ESCAPE:
                    running = false;
                    break;

                case SDLK_SPACE:
                    renderer.togglePause();
                    break;

                case SDLK_UP:
                    renderer.increaseSpeed();
                    break;

                case SDLK_DOWN:
                    renderer.decreaseSpeed();
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
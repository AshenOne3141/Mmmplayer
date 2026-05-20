#include "../include/Window.h"
#include "../include/Renderer.h"

int main(int argc, char* argv[]) {

    Window window;

    if (!window.initialise())
        return -1;

    Renderer renderer(window);

    if (!renderer.initialise())
        return -1;

    bool running = true;

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                switch (event.key.key)
                {
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

        renderer.render();

        SDL_Delay(16);
    }

    return 0;
}
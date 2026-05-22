#pragma once
#include <iostream>
class UI {

public:

    bool initialise(
        SDL_Window* window,
        SDL_Renderer* renderer
    );

    void beginFrame();

    std::string render(bool &running);

    void shutdown();
    void processEvents(SDL_Event* event);
private:
    SDL_Renderer* renderer;
};
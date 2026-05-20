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

    while (running) {

        running = window.processEvents();

        renderer.render();
    }

    return 0;
}
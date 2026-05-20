#pragma once
#include <SDL3/SDL.h>
#include "Window.h"
class Renderer {
public:
	Renderer(Window& window);
	~Renderer();
	bool initialise();
	void render();
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Window& window;



};
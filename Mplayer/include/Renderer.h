#pragma once
#include <SDL3/SDL.h>
#include "Window.h"
#include <vector>
class Renderer {
public:
	Renderer(Window& window);
	~Renderer();
	bool initialise();
	void render();
	void togglePause();
	void increaseSpeed();
	void decreaseSpeed();
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Window& window;
	std::vector<uint32_t> pixels;
	int frameNumber;
	bool paused;
	int speed;



};
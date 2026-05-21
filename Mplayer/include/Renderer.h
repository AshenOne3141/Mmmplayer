#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "../include/Renderer.h"
#include "../include/Window.h"
#include <vector>
#include "../include/FFmpegDecoder.h"
class Renderer {
public:
	Renderer(Window& window);
	~Renderer();
	bool initialise(int w,int h);
	void render(AVFrame* rgbFrame);
	void togglePause();
	void increaseSpeed();
	void decreaseSpeed();
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Window& window;
	int frameNumber;
	bool paused;
	int speed;



};
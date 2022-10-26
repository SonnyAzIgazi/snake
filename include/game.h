#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include "snake.h"
#include <vector>

class Game {

public:
	int status = 0;

	Game();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake* snake;
	SDL_Rect* food;

	bool running = true;

	void checkFood();
	bool checkLose();
	void checkOutside();
	void Render();
	void EventHandling();
	void OwnEvents(SDL_Event* event);
	void KeyboardHandling(SDL_Event* event);
};
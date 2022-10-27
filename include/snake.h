#pragma once
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#define M_UP 0
#define M_DOWN 1
#define M_LEFT 2
#define M_RIGHT 3


class Snake {
	public:
		SDL_Texture* texture;
		std::vector<SDL_Rect*> nodes;

		char currentMovement;
		char lastMovement;
		bool addTail;

		Snake(int x, int y, Uint8 tail);


		void tick();
};
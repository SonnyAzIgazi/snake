#include "game.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <tuple>
#include <vector>

#define WINDOW_SIZE 600

Uint32 my_callbackfunc(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes a function
    into the queue, and causes our callback to be called again at the
    same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

Game::Game() {
	this->snake = new Snake(5, 5, 1);
	this->food = new SDL_Rect;
	this->food->h = 60;
	this->food->w = 60;
	this->food->x = 0;
	this->food->y = 0;

	std::srand(time(0));

	if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_TIMER) == 0) {
		if (SDL_CreateWindowAndRenderer(WINDOW_SIZE+1, WINDOW_SIZE+1, 0, &window, &renderer) == 0) {
			 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			
			SDL_TimerID my_timer_id = SDL_AddTimer((Uint32)300, (SDL_TimerCallback)my_callbackfunc, (void *)12);

			while (running) {
				this->Render();
				this->EventHandling();
			}
		}
		if (renderer) {
			SDL_DestroyRenderer(renderer);
		}
		if (window) {
			SDL_DestroyWindow(window);
		}
	}
	SDL_Quit();
	std::cout << this->snake->nodes.size() << std::endl;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int x = 0; x <= 10; x++) {
		SDL_RenderDrawLine(renderer, x*WINDOW_SIZE/10, 0, x*WINDOW_SIZE/10, WINDOW_SIZE);
	}
	for (int y = 0; y <= 10; y++) {
		SDL_RenderDrawLine(renderer, 0, y*WINDOW_SIZE/10, WINDOW_SIZE, y*WINDOW_SIZE/10);
	}

	for (auto &node : this->snake->nodes) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, node);
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, this->food);
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderPresent(renderer);
}

void Game::EventHandling() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			KeyboardHandling(&event);
		} else if (event.type == SDL_QUIT) {
			this->running = false;
		} else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
			//OnClick(&event);
		} else if (event.type == SDL_USEREVENT) {
			this->OwnEvents(&event);
		}
	}
}

void Game::checkFood() {
	if (this->snake->nodes[0]->x == this->food->x && this->snake->nodes[0]->y == this->food->y) {
		this->snake->addTail = true;

		int x = std::rand() % 10;
		int y = std::rand() % 10;
		
		this->food->x = x * 60;
		this->food->y = y * 60;
	}
}

bool Game::checkLose() {
	struct Point {
		int x;
		int y;
	};

	std::vector<std::tuple<int, int>> alreadyPoints = std::vector<std::tuple<int, int>>();
	for (auto &node: this->snake->nodes) {
		std::tuple<int, int> thisPoint = std::tuple<int, int>(node->x, node->y);
		if (std::count(alreadyPoints.begin(), alreadyPoints.end(), thisPoint)) {
			return true;
		}
		alreadyPoints.push_back(thisPoint);
	}
	return false;
}

void Game::checkOutside() {
	int x = this->snake->nodes.front()->x;
	int y = this->snake->nodes.front()->y;
	x /= 60;
	y /= 60;

	if (x < 0) {
		this->snake->nodes.front()->x = 60*9;
	}
	if (x > 9) {
		this->snake->nodes.front()->x = 0;
	}
	if (y < 0) {
		this->snake->nodes.front()->y = 60*9;
	}
	if (y > 9) {
		this->snake->nodes.front()->y = 0;
	}

}

void Game::OwnEvents(SDL_Event* event) {
	if (event->user.code == 0) {
		this->checkFood();
		this->snake->tick();
		this->checkFood();
		if (this->checkLose()) {
			this->running = false;
		}
		this->checkOutside();
	}
}

void Game::KeyboardHandling(SDL_Event* event) {
	switch (event->key.keysym.scancode) {
		case SDL_SCANCODE_Q:
			this->running = false;
			break;
		case SDL_SCANCODE_W:
			this->snake->currentMovement = M_UP;
			break;
		case SDL_SCANCODE_A:
			this->snake->currentMovement = M_LEFT;
			break;
		case SDL_SCANCODE_S:
			this->snake->currentMovement = M_DOWN;
			break;
		case SDL_SCANCODE_D:
			this->snake->currentMovement = M_RIGHT;
			break;
	}
}
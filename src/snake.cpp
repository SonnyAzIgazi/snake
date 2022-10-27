#include "snake.h"
#include <SDL2/SDL_rect.h>
#include <vector>
#include <math.h>
#include <algorithm>



Snake::Snake(int x, int y, Uint8 tail) {
	SDL_Rect* head = new SDL_Rect;
	head->x = x * 60;
	head->y = y * 60;
	head->h = 60;
	head->w = 60;
	this->nodes.push_back(head);

	for (Uint8 i = 0; i < tail; ++i) {
		SDL_Rect* rect = new SDL_Rect;
		rect->x = (x+i+1) * 60;
		rect->y = y * 60;
		rect->h = 60;
		rect->w = 60;
		this->nodes.push_back(rect);
	}
}

void Snake::tick() {
	int x = 0;
	int y = 0;
	
	switch (this->currentMovement) {
		case M_UP:
			y--;
			break;
		case M_DOWN:
			y++;
			break;
		case M_LEFT:
			x--;
			break;
		case M_RIGHT:
			x++;
			break;
		default:
			x++;
			break;
	}

	lastMovement = this->currentMovement;

	int lastX = nodes[0]->x;
	int lastY = nodes[0]->y;

	nodes[0]->x += x*60;
	nodes[0]->y += y*60;

	for(std::vector<SDL_Rect*>::iterator node = this->nodes.begin() + 1; node != this->nodes.end(); ++node) {
		int currentX = (*node)->x;
		int currentY = (*node)->y;

		(*node)->x = lastX;
		(*node)->y = lastY;
		lastX = currentX;
		lastY = currentY;
	}

	if (this->addTail) {
		SDL_Rect* newTail = new SDL_Rect;
		newTail->h = 60;
		newTail->w = 60;
		newTail->x = lastX;
		newTail->y = lastY;
		nodes.push_back(newTail);
		this->addTail = false;
	}

	return;
}
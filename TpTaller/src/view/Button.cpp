/*
 * Button.cpp
 *
 *  Created on: 25/03/2013
 *      Author: tomas
 */

#include <view/Button.h>

using namespace std;
Button::Button(SDL_Surface* pressed, SDL_Surface* released, SDL_Rect pos,
		MenuEvent event) {
	this->pressed = pressed;
	this->released = released;
	this->pos = pos;
	this->state = BUTTON_RELEASED;
	this->event = event;
}

bool Button::isPressed() {
	if (state == BUTTON_PRESSED)
		return true;

	return false;
}

void Button::changeState() {
	if (state == BUTTON_PRESSED) {
		state = BUTTON_RELEASED;
	} else {
		state = BUTTON_PRESSED;
	}
}

void drawImage(SDL_Surface* image, SDL_Surface* screen, SDL_Rect* pos) {
	SDL_BlitSurface(image, NULL, screen, pos);
	SDL_UpdateRects(screen, 1, pos);
}

void Button::draw(SDL_Surface* screen) {
	switch (state) {
	case BUTTON_PRESSED:
		drawImage(pressed, screen, &pos);
		break;
	case BUTTON_RELEASED:
		drawImage(released, screen, &pos);
		break;
	}
}

Button::~Button() {
	SDL_FreeSurface(pressed);
	SDL_FreeSurface(released);
}


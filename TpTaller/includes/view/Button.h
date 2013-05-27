/*
 * Button.h
 *
 *  Created on: 25/03/2013
 *      Author: tomas
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Events.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>

using namespace std;

typedef enum {
	BUTTON_PRESSED, BUTTON_RELEASED, BUTTON_HOVERED
} ButtonState;

class Button {
public:
	Button(SDL_Surface* pressed, SDL_Surface* released, SDL_Rect pos,
			MenuEvent event);
	Button(SDL_Surface* pressed, SDL_Surface* released, SDL_Surface* hovered, SDL_Rect pos,
				MenuEvent event);
	SDL_Rect pos;
	SDL_Surface* released;
	SDL_Surface* pressed;
	SDL_Surface* hovered;
	MenuEvent event;

	bool isPressed();
	void changeState();
	void changeState(ButtonState aState);
	void draw(SDL_Surface* screen);

	virtual ~Button();
private:
	ButtonState state;
};

#endif /* BUTTON_H_ */

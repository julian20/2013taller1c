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

namespace std {

typedef enum{
	BUTTON_PRESSED,
	BUTTON_RELEASED
}ButtonState;

class Button {
public:
	Button(SDL_Surface* pressed, SDL_Surface* released, SDL_Rect pos, MenuEvent event);

	SDL_Rect pos;
	SDL_Surface* released;
	SDL_Surface* pressed;
	MenuEvent event;

	bool isPressed();
	void changeState();

	void draw(SDL_Surface* screen);

	virtual ~Button();
private:
	ButtonState state;
};

} /* namespace std */
#endif /* BUTTON_H_ */

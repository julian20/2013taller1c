/*
 * Menu.h
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */

#ifndef MENU_H_
#define MENU_H_

#include <SDL/SDL.h>

namespace std {

typedef enum {
	NEWGAME_EVENT,
	EXIT_EVENT,
	CONFIG_EVENT
}MenuEvent;

class Menu {
public:
	Menu();
	void showMenu();
	MenuEvent getEvent();
	void runConfigMenu();
	virtual ~Menu();
private:
	SDL_Surface *screen;
};



} /* namespace std */
#endif /* MENU_H_ */

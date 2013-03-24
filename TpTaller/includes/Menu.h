/*
 * Menu.h
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */

#ifndef MENU_H_
#define MENU_H_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace std {

typedef enum {
	NEWGAME_EVENT,
	EXIT_EVENT,
	CONFIG_EVENT,
	NOTHING_EVENT
}MenuEvent;

class Menu {
public:
	Menu();
	void showMenu();
	MenuEvent getEvent();
	void runConfigMenu();
	virtual ~Menu();
private:
	void startMusic();
	void startLaugh();
	SDL_Surface *screen;
	Mix_Music *musica;
	Mix_Chunk *sonido;
};



} /* namespace std */
#endif /* MENU_H_ */

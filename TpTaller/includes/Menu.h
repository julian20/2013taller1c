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

#define NUM_BUTTONS 2

typedef enum {
	NEWGAME_EVENT,
	EXIT_EVENT,
	CONFIG_EVENT,
	NOTHING_EVENT
}MenuEvent;

class Menu {
public:
	Menu();
	void init();
	MenuEvent run();
	void runConfigMenu();
	virtual ~Menu();
private:
	void startScreen();
	void startButtons();
	void startMusic();
	void startLaugh();
	void startVoice();

	void checkPressedButton(SDL_Event event);
	MenuEvent checkReleasedButton(SDL_Event event);
	SDL_Surface *screen;
	Mix_Music *musica;
	Mix_Chunk *sonido;
	Mix_Chunk *darknessVoice;
};



} /* namespace std */
#endif /* MENU_H_ */

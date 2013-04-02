/*
 * Menu.h
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */

#ifndef MENU_H_
#define MENU_H_

#include <Events.h>

#include <model/persistence/ConfigurationReader.h>

#include <view/MenuView.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace std {

#define NUM_BUTTONS 2

class Menu {
public:
	Menu();
	MenuEvent run();
	void runConfigMenu();
	virtual ~Menu();
private:
	MenuView* view;

	void checkPressedButton(SDL_Event event);
	MenuEvent checkReleasedButton(SDL_Event event);

};

} /* namespace std */
#endif /* MENU_H_ */

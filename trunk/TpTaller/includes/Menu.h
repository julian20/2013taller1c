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
#include <view/configuration/GameConfiguration.h>
#include <view/MenuView.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

using namespace std;

#define NUM_BUTTONS 4

class Menu {
public:
	Menu(GameConfiguration* gameConfig);
	MenuEvent run();
	void close();
	void runConfigMenu();
	virtual ~Menu();
private:
	MenuView* view;

	bool closed;

	void checkPressedButton(SDL_Event event);
	MenuEvent checkReleasedButton(SDL_Event event);
	void checkHoveredButton(SDL_Event event);

};

#endif /* MENU_H_ */

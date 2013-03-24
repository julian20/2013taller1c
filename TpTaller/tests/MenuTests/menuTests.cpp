/*
 * menuTests.cpp
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */


#include <Menu.h>

using namespace std;


// Change to main and run the makefile in the terminal.
int main2(){

	Menu* menu = new Menu();
	menu->showMenu();
	SDL_Event event;

	while (event.type != SDL_QUIT)
		SDL_PollEvent(&event);
	delete menu;

	return 0;
}


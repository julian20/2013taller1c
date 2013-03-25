/*
 * menuTests.cpp
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */
 

#include <Menu.h>

using namespace std;


// Change to main and run the makefile in the terminal.
int main(){

	Menu* menu = new Menu();
	while (true){

			MenuEvent event = menu->getEvent();
			switch (event){
			case NEWGAME_EVENT:
				break;
			case CONFIG_EVENT:
				menu->runConfigMenu();
				break;
			case EXIT_EVENT:
				menu->~Menu();
				exit(1);
			default:
				break;
				//TODO: add more
			}
			menu->showMenu();
	}
	delete menu;

	return 0;
}


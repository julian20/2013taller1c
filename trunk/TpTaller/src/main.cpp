/*
 * file: main.cpp
 *
 *  Created on: 22/03/2013
 *  Project: TpTaller
 *
 */

#include <cstdlib>
#include <string>
#include <stdio.h>
#include <model/ConfigurationReader.h>
#include <Game.h>
#include <Menu.h>

#define CONFIGURATION_FILE "./configuration/entities.yaml"

using namespace std;

/*
 *
 */
int main(int argc, char** argv) {

	// Lectura del archivo de configuracion
	ConfigurationReader* cfgReader = new ConfigurationReader();
	cfgReader->loadConfiguration(CONFIGURATION_FILE);

	Menu* menu = new Menu();
	Game* game = new Game(cfgReader);

	delete cfgReader;


	while (true){

		MenuEvent event = menu->getEvent();
		switch (event){
		case NEWGAME_EVENT:
			game->run();
			break;
		case CONFIG_EVENT:
			menu->runConfigMenu();
			break;
		case EXIT_EVENT:
			menu->~Menu();
			exit(1);
			//TODO: add more
		}
		menu->showMenu();
	}

	delete game;
	delete menu;



	return 0;
}




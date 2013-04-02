/*
 * file: main.cpp
 *
 *  Created on: 22/03/2013
 *  Project: TpTaller
 *
 */

#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <model/persistence/ConfigurationReader.h>
#include <Game.h>
#include <Menu.h>

#define CONFIGURATION_FILE "./configuration/entities.yaml"

using namespace std;

/*
 *
 */
int main(int argc, char** argv) {
	// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	cfgReader.loadConfiguration(CONFIGURATION_FILE);

	Menu menu = Menu();
	Game game = Game(&cfgReader);

	MenuEvent event = NOTHING_EVENT;
	bool gameRunning = false;

	while (event != EXIT_EVENT) {

		event = (gameRunning) ? game.run() : menu.run();
		switch (event) {
		case NOTHING_EVENT:
			break;
		case EXIT_EVENT:
			break;
		case NEWGAME_EVENT:
			//Aca inicio el juego
			menu.~Menu();
			gameRunning = true;	// QUE CODIGO LOCOOOO PAPÁ
			break;
		case CONFIG_EVENT:
			//Aca inicio el menu de configuraciones
			cout << "Se inicio el menu de configuraciones" << endl;
			break;

		}

	}

	return 0;
}


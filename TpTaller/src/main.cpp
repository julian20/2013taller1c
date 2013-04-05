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
#define OUTPUT_FILENAME "configuration/parserOutput.yaml"

using namespace std;

void initGame() {
	// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	PersistentConfiguration configuration = cfgReader.loadConfiguration(
			CONFIGURATION_FILE, OUTPUT_FILENAME);
	Game game = Game(&configuration);
	game.run();
	game.~Game();
}

/*
 *
 */
int main(int argc, char** argv) {

	Menu menu = Menu();

	MenuEvent event = NOTHING_EVENT;
	while (event != EXIT_EVENT) {

		event = menu.run();
		switch (event) {
		case NOTHING_EVENT:
			break;
		case EXIT_EVENT:
			break;
		case NEWGAME_EVENT:
			//Aca inicio el juego
			menu.~Menu();
			initGame();
			break;
		case CONFIG_EVENT:
			//Aca inicio el menu de configuraciones
			cout << "Se inicio el menu de configuraciones" << endl;
			break;

		}

	}

	return 0;
}


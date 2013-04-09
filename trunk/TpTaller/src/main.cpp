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

void initGame(PersistentConfiguration* configuration) {
	Game* game = new Game(configuration);
	game->run();
	delete game;
}

/*
 *
 */
int main(int argc, char** argv) {

	// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	PersistentConfiguration configuration = cfgReader.loadConfiguration(
			CONFIGURATION_FILE, OUTPUT_FILENAME);
	initGame(&configuration);
//	Menu* menu = new Menu(configuration.getAnimationConfiguration());

//	MenuEvent event = NOTHING_EVENT;
//	while (event != EXIT_EVENT) {
//
//		event = menu->run();
//		switch (event) {
//		case NOTHING_EVENT:
//			break;
//		case EXIT_EVENT:
//			break;
//		case NEWGAME_EVENT:
//			//Aca inicio el juego
//			menu->close();
//			initGame(&configuration);
//			event = EXIT_EVENT;
//			break;
//		case CONFIG_EVENT:
//			//Aca inicio el menu de configuraciones
//			cout << "Se inicio el menu de configuraciones" << endl;
//			break;
//
//		}
//
//	}
//
//	delete menu;
	return 0;
}


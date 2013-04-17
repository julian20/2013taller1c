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
#include <model/persistence/ConfigurationReader.h>
#include <model/Logs/Logs.h>
#include <Game.h>
#include <Menu.h>

#include <SDL/SDL.h>

#define CONFIGURATION_FILE "./configuration/entities.yaml"
#define OUTPUT_FILENAME "configuration/parserOutput.yaml"
#define DEFAULT_CONFIGURATION_FILE "./configuration/.entitiesDefault.yaml"

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
	Logs unLog;
	Logs::openFile();
	Logs::logErrorMessage(string("************Program Starting**************** "));
	SDL_Init(SDL_INIT_EVERYTHING);

	// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	try {
		PersistentConfiguration configuration = cfgReader.loadConfiguration(CONFIGURATION_FILE, OUTPUT_FILENAME);
		initGame(&configuration);
	} catch (std::exception& e) {
		unLog.logErrorMessage(string("Configuration syntax error, ") + e.what() +string (". Loading default configuration."));
		PersistentConfiguration configuration = cfgReader.loadConfiguration(DEFAULT_CONFIGURATION_FILE, OUTPUT_FILENAME);
		initGame(&configuration);
	}

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

	SDL_Quit();
	Logs::logErrorMessage(string("************Program Ended**************** "));
	Logs::closeFile();
//	unLog.logErrorMessage(string("******************Program Finished***************"));
	return 0;
}


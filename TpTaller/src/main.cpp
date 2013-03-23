/*
 * file: main.cpp
 *
 *  Created on: 22/03/2013
 *  Project: TpTaller
 *
 */

#include <cstdlib>
#include <string>
#include <model/ConfigurationReader.h>
#include <model/Game.h>

#define CONFIGURATION_FILE "../configuration/entities.yaml"

using namespace std;

/*
 *
 */
int main(int argc, char** argv) {

	// Lectura del archivo de configuracion
	ConfigurationReader* cfgReader = new ConfigurationReader();
	cfgReader->loadConfiguration(CONFIGURATION_FILE);

	//Inicializamos todo lo que haya que inicializar
	Game* game = new Game(cfgReader);

	// GameLoop
	game->run();

	// Liberamos recursos
	delete game;

	return 0;
}




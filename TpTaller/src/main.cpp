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
#include <networking/Client.h>
#include <networking/Server.h>

#include <SDL/SDL.h>

#define CONFIGURATION_FILE "./configuration/entities.yaml"
#define OUTPUT_FILENAME "configuration/parserOutput.yaml"
#define DEFAULT_CONFIGURATION_FILE "./configuration/.entitiesDefault.yaml"
#define CLIENT_CONFIGURATION_FILE "./configuration/clientPlayerConfiguration.yaml"

using namespace std;

void initGame(PersistentConfiguration* configuration) {
	Game* game = new Game(configuration, false);
	game->run();
	delete game;
}

void initMultiplayerGame(PersistentConfiguration* configuration,
		string& playerName, string& playerType) {

	std::string serverIP =
			configuration->getAnimationConfiguration()->getServerIP();
	unsigned int serverPort =
			configuration->getAnimationConfiguration()->getServerPort();

	Client* client = new Client(serverIP, serverPort);
	client->downloadFiles();

	ConfigurationReader newReader = ConfigurationReader();
	PersistentConfiguration downloadedConfig = newReader.loadConfiguration(
			CONFIGURATION_FILE, OUTPUT_FILENAME);

	Game* game = new Game(&downloadedConfig, true);

	game->getPlayerView()->setShowableName(string(playerName));
//	game->getPlayerView()->setName(string(playerType));
	game->getPlayerView()->getPersonaje()->setName(string(playerName));

	client->setGame(game);
	client->initPlayerInfo(game->getPlayerView());
	client->run();
	game->run();
	delete client;
	delete game;
}

void initServer(PersistentConfiguration* configuration) {

	unsigned int serverPort =
			configuration->getAnimationConfiguration()->getServerPort();

	MultiplayerGame* game = new MultiplayerGame(configuration);
	Server* server = new Server(serverPort);
	server->run(game);
	delete server;
}

void initMenu(PersistentConfiguration* configuration, string& playerName,
		string& playerType) {

	Menu* menu = new Menu(configuration->getAnimationConfiguration());

	MenuEvent event = NOTHING_EVENT;
	while (event != EXIT_EVENT) {

		event = menu->run();
		switch (event) {
		case NOTHING_EVENT:
			break;
		case EXIT_EVENT:
			break;
		case NEWGAME_EVENT:
			//Aca inicio el juego
			menu->close();
			initGame(configuration);
			event = EXIT_EVENT;
			break;
		case CONFIG_EVENT:
			//Aca inicio el menu de configuraciones
			cout << "Se inicio el menu de configuraciones" << endl;
			break;
		case MULTIPLAYER_GAME_EVENT:
			menu->close();
			initMultiplayerGame(configuration, playerName, playerType);
			event = EXIT_EVENT;
			break;
		case SERVER_EVENT:
			menu->close();
			initServer(configuration);
			event = EXIT_EVENT;
			break;

		}

	}

	delete menu;
}

/*
 *
 */
int main(int argc, char** argv) {

	Logs unLog;
	Logs::openFile();
	Logs::logErrorMessage(
			string("************Program Starting**************** "));
	SDL_Init(SDL_INIT_EVERYTHING);

	string playerName("NewPlayer");
	string playerType("NewPlayer");

	if (argc > 1) {
		playerName = string(argv[1]);
	}
	if (argc > 2) {
		playerType = string(argv[2]);
	}

// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	try {
		PersistentConfiguration configuration = cfgReader.loadConfiguration(
				CONFIGURATION_FILE, OUTPUT_FILENAME);
		initMenu(&configuration, playerName, playerType);
	} catch (std::exception& e) {
		unLog.logErrorMessage(
				string("Configuration syntax error, ") + e.what()
						+ string(". Loading default configuration."));
		PersistentConfiguration configuration = cfgReader.loadConfiguration(
				DEFAULT_CONFIGURATION_FILE, OUTPUT_FILENAME);
		initMenu(&configuration, playerName, playerType);
	}

	SDL_Quit();
	Logs::logErrorMessage(string("************Program Ended**************** "));
	Logs::closeFile();
	return 0;
}


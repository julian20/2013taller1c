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
#include <view/sound/SoundEffectHandler.h>
#include <model/Logs/Logs.h>
#include <Game.h>
#include <Menu.h>
#include <networking/Client.h>
#include <networking/Server.h>
#include <networking/SinglePlayerGame/SinglePlayerServer.h>

#include <SDL/SDL.h>

#include <vector>

#define SINGLE_PLAYER_CONFIG_FILE "./configuration/singleplayer/entities.yaml"
#define DEFAULT_CONFIGURATION_FILE "./configuration/.entitiesDefault.yaml"

#define CLIENT_CONFIG_FILE "./configuration/client-config/clientLocalConfig.yaml"
#define CLIENT_RECEIVED_FILE "./configuration/client-received/entities.yaml"
#define CLIENT_MAP_FILE "./configuration/client-received/map.yaml"

#define SERVER_CONFIG_FILE "./configuration/server/serverLocalConfig.yaml"
#define SERVER_SEND_MAP_LOCATION "./sendFiles/configuration/client-received/entities.yaml"

#define OUTPUT_FILENAME "configuration/parserOutput.yaml"

#define COPY_MAP_SCRIPT "./copyServerSendFile.sh"

using namespace std;

void initGame() {

	ConfigurationReader configReader = ConfigurationReader();
	PersistentConfiguration configuration = configReader.loadConfiguration(SINGLE_PLAYER_CONFIG_FILE,OUTPUT_FILENAME);

	std::string serverIP = configuration.getAnimationConfiguration()->getServerIP();
	unsigned int serverPort = configuration.getAnimationConfiguration()->getServerPort();

	MultiplayerGame* MPgame = new MultiplayerGame(&configuration);
	MPgame->addNewPlayer(new Player(), new Coordinates(9,9));
	SinglePlayerServer* server = new SinglePlayerServer(serverPort);
	server->run(MPgame);

	Client* client = new Client(serverIP, serverPort);
	Game* game = new Game(&configuration, true);
	client->setGame(game);
	client->initPlayerInfo(game->getPlayerView());
	client->run();

	delete client;
	delete game;
	delete MPgame;
	delete server;
}

void initMultiplayerGame(string& playerName, string& playerType) {

	ConfigurationReader clientConfigReader = ConfigurationReader();
	PersistentConfiguration configuration = clientConfigReader.loadConfiguration(CLIENT_CONFIG_FILE,OUTPUT_FILENAME);

	std::string serverIP =
			configuration.getAnimationConfiguration()->getServerIP();
	unsigned int serverPort =
			configuration.getAnimationConfiguration()->getServerPort();

	Client* client = new Client(serverIP, serverPort);
	client->downloadFiles();

	// Reemplazo el tag en el archivo de configuracion por
	// el player type.
	string command = string("sed \"s/NAME_TAG/");
	command += playerType;
	command += string("/g\" ");
	command += string(CLIENT_RECEIVED_FILE);
	command += string(" > ");
	command += string(CLIENT_MAP_FILE);
	system(command.c_str());

	ConfigurationReader newReader = ConfigurationReader();
	PersistentConfiguration downloadedConfig = newReader.loadConfiguration(
			CLIENT_MAP_FILE, OUTPUT_FILENAME);

	Game* game = new Game(&downloadedConfig, true);

	game->getPlayerView()->setShowableName(string(playerName));
	game->getPlayerView()->getPersonaje()->setName(string(playerName));

	client->setGame(game);
	client->initPlayerInfo(game->getPlayerView());
	client->run();

	delete client;
	delete game;

}

void initServer() {

	ConfigurationReader serverConfigReader = ConfigurationReader();
	PersistentConfiguration configuration = serverConfigReader.loadConfiguration(SERVER_CONFIG_FILE,OUTPUT_FILENAME);

	string command = string(COPY_MAP_SCRIPT) + string(" ") + string(SERVER_CONFIG_FILE) + string(" ") +  string(SERVER_SEND_MAP_LOCATION);
	system(command.c_str());


	unsigned int serverPort =
			configuration.getAnimationConfiguration()->getServerPort();

	MultiplayerGame* game = new MultiplayerGame(&configuration);
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
			initGame();
			event = EXIT_EVENT;
			break;
		case CONFIG_EVENT:
			//Aca inicio el menu de configuraciones
			cout << "Se inicio el menu de configuraciones" << endl;
			break;
		case MULTIPLAYER_GAME_EVENT:
			menu->close();
			SoundEffectHandler::initialize();
			initMultiplayerGame(playerName, playerType);
			event = EXIT_EVENT;
			break;
		case SERVER_EVENT:
			menu->close();
			initServer();
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
	Logs::logErrorMessage(string("************Program Starting**************** "));
	SDL_Init(SDL_INIT_EVERYTHING);

	string playerName("NewPlayer");
	string playerType("NewPlayer");

	if (argc > 1) {
		playerName = string(argv[1]);
	}
	if (argc > 2) {
		playerType = string(argv[2]);
	}

	std::vector<std::string> validList;
	validList.push_back(string("NewPlayer"));
	validList.push_back(string("NewPlayerBlue"));

	bool valid = false;

	for( unsigned i = 0 ; i < validList.size() ; i++ ) {
		if( playerType == validList[i] ) valid = true;
	}

	if( ! valid ) playerType = string("NewPlayer");

	std::cout << "Selected player: " << playerType << std::endl;

// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	try {
		PersistentConfiguration configuration = cfgReader.loadConfiguration(
				SINGLE_PLAYER_CONFIG_FILE, OUTPUT_FILENAME);
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


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

char *leer_linea(FILE* archivo) {
	int tam = 256, i = 0;
	char *linea = (char*) malloc(sizeof(char) * tam);
	char letra;
	do {
		letra = fgetc(archivo);
		linea[i] = letra;
		if (tam == i + 1) {
			tam += 10;
			char *aux = (char*) realloc(linea, sizeof(char) * tam);
			if (!aux) {
				linea[i] = '\0';
				return linea;
			} else {
				linea = aux;
			}
		}
		i++;
	} while (letra != '\n' && letra != EOF);
	linea[i - 1] = '\0';
	return linea;
}

void generateClientYAML(string serverYAML, string clientYAML, string output) {

	FILE* serverFILE = fopen(serverYAML.c_str(), "r");
	FILE* clientFILE = fopen(clientYAML.c_str(), "r");
	FILE* outputFILE = fopen(output.c_str(), "w");

	if (serverFILE != NULL && clientFILE != NULL && outputFILE != NULL) {

		bool flag = true;
		while (!feof(serverFILE)) {
			char* lineRead;
			if (flag) {
				lineRead = leer_linea(clientFILE);

				if (strcmp(lineRead, "- entityViews:") == 0
						|| strcmp(lineRead, "") == 0) {
					flag = false;
					if (strcmp(lineRead, "") == 0) {
						fprintf(outputFILE, "- entityLocations:\n");
						flag = false;
						free(lineRead);
						continue;
					}
					free(lineRead);
					continue;
				}
				fprintf(outputFILE, "%s\n", lineRead);
				free(leer_linea(serverFILE));
				free(lineRead);
			} else {
				lineRead = leer_linea(serverFILE);
				if (strcmp(lineRead, "- playerLocations:") == 0) {
					flag = true;
					free(lineRead);
					continue;
				}
				fprintf(outputFILE, "%s\n", lineRead);
				free(lineRead);
			}

		}

	} else {
		Logs::logErrorMessage(
				string("No se pudo leer los archivos de configuracion"));
	}

	fclose(serverFILE);
	fclose(clientFILE);
	fclose(outputFILE);

}

void initMultiplayerGame(PersistentConfiguration* configuration) {

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
	client->setGame(game);
	client->initPlayerInfo(game->getPlayerView());
	client->run();
	game->run();
	delete game;
	delete client;
}

void initServer(PersistentConfiguration* configuration) {

	std::string serverIP =
			configuration->getAnimationConfiguration()->getServerIP();
	unsigned int serverPort =
			configuration->getAnimationConfiguration()->getServerPort();

	MultiplayerGame* game = new MultiplayerGame(configuration);
	Server* server = new Server(serverIP, serverPort);
	server->run(game);
	delete server;
}

void initMenu(PersistentConfiguration* configuration) {

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
			initMultiplayerGame(configuration);
			break;
		case SERVER_EVENT:
			menu->close();
			initServer(configuration);
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

// Lectura del archivo de configuracion
	ConfigurationReader cfgReader = ConfigurationReader();
	try {
		PersistentConfiguration configuration = cfgReader.loadConfiguration(
				CONFIGURATION_FILE, OUTPUT_FILENAME);
		initMenu(&configuration);
	} catch (std::exception& e) {
		unLog.logErrorMessage(
				string("Configuration syntax error, ") + e.what()
						+ string(". Loading default configuration."));
		PersistentConfiguration configuration = cfgReader.loadConfiguration(
				DEFAULT_CONFIGURATION_FILE, OUTPUT_FILENAME);
		initMenu(&configuration);
	}

	SDL_Quit();
	Logs::logErrorMessage(string("************Program Ended**************** "));
	Logs::closeFile();
	return 0;
}


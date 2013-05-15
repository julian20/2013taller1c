/*
 * Server.cpp
 *
 *  Created on: 26/04/2013
 *      Author: tomas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <SDL/SDL.h>

#include <model/Logs/Logs.h>
#include <networking/Server.h>
#include <networking/ComunicationUtils.h>
#include <networking/PlayerInfo.h>

// TODO: LEER ESTO DE UN YAML
#define BACKLOG     10  /* Passed to listen() */
#define READING_SIZE 1024
#define ALIVE_SIGNAL "ALIVE"
#define OK 0
#define ERROR -1

using namespace std;

/* ***************************************************************** */
/* *************  FUNCIONES EJECUTADAS EN LOS THREADS ************** */
/* ***************************************************************** */


// Funcion que ejecuta al conectarse cada client
void* handle(void* par){

	/* send(), recv(), close() */
		ThreadParameter* parameter = (ThreadParameter*) par;
		int clientSocket = parameter->clientID;
		Server* server = parameter->server;
		MultiplayerGame* game = server->getGame();
		//Lo primero que hago es mandar el mapa.
		std::vector<std::string> withBase = server->listFilesInDirectoryWithBase("sendFiles");
		std::vector<std::string> withoutBase = server->listFilesInDirectory("sendFiles");

//		std::cout << withoutBase[0] << std::endl;
	//server->sendFile(withBase[0],withoutBase[0],clientSocket);
	server->sendFiles(withBase, withoutBase, clientSocket);
	//server->sendFile(withBase[0],withoutBase[0],clientSocket);

		// Manda las imagenes y sonidos necesarios que se utilizaran.
		//TODO : sendResources(sockID);

	map<int, string> sended;
	PlayerInfo* info = server->recieveNewPlayer(clientSocket);
	string playerName = info->getPlayer()->getName();

		int result = server->isNameAbilivable(playerName);
		server->sendAproval(clientSocket,result);
		if (result != OK){
			playerName = server->getAbilivableName(playerName);
			server->sendNewName(clientSocket, playerName);
			info->setName(playerName);
			info->getPlayer()->setName(playerName);
		}

		sended.insert(pair<int, string>(clientSocket,playerName));
		server->addPlayerToGame(clientSocket,info);


		cout << playerName << " has conected.. " << endl;
		ChatServer* serverChat=server->getChat();
		serverChat->setGame(game);
		serverChat->addPlayerToChat(clientSocket,playerName);
		bool playing = true;

		while (playing){

		playing = server->exchangeAliveSignals(clientSocket);
		if (!playing)
			break;


			server->sendNewPlayers(clientSocket, &sended);
			vector<PlayerEvent*> events = server->recvEvents(clientSocket);
			if (!events.empty()) game->addEventsToHandle(playerName,events);
			server->getPlayersUpdates();
			server->sendPlayersUpdates(clientSocket, playerName);
			serverChat->getChatUpdates();
			serverChat->sendChatUpdates(clientSocket, playerName);
		}

		server->disconectPlayer(clientSocket,playerName);

		return NULL;

}

// Funcion que corre la logica de actualizaciones de los jugadores
void* runGameBackEnd(void* parameter){
	MultiplayerGame* game = (MultiplayerGame*) parameter;
	game->run();
	return NULL;
}

// Funcion que lee eventos para saber cuando cerrar el servidor
void* readEvents(void* par ){

	while (true){

		SDL_Event event;
		while (SDL_PollEvent(&event)){

			if ( (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) || (event.type == SDL_QUIT ) ){
				exit(0);
			}
		}

	}

	return NULL;

}


/* ***************************************************************** */
/* *************************  CLASE SERVER ************************* */
/* ***************************************************************** */

/* *********************** SERVER CONTRUCTOR ************************ */

Server::Server(string host, int port) {
	struct addrinfo hints, *res;
	int reuseaddr = 1; /* True */

	/* Get the address info */
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	stringstream ssport;
	ssport << port;

	if (getaddrinfo(host.c_str(), ssport.str().c_str(),&hints, &res) != 0) {
		Logs::logErrorMessage("Servidor: Error al obtener la informacion de direccion");
		exit(1);
	}

	/* Create the socket */
	serverID = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (serverID == -1) {
		Logs::logErrorMessage("Servidor: Error al inicializar el servidor");
		exit(1);
	}

	/* Enable the socket to reuse the address */
	if (setsockopt(serverID, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
		Logs::logErrorMessage("Servidor: Error al setear la reutilizacion de direcciones");
		exit(1);
	}

	/* Bind to the address */
	if (bind(serverID, res->ai_addr, res->ai_addrlen) == -1) {
		Logs::logErrorMessage("Servidor: Error de asignacion de direccion");
		exit(1);
	}
	this->chat = new ChatServer();
	this->chat->setSocketId(serverID);
	this->chat->setMaxConnections(BACKLOG);
//	printf("creo el chatServer\n");
	freeaddrinfo(res);

	SDL_Init(SDL_INIT_JOYSTICK);

}


/* **************************** SERVER RUN ************************** */

void Server::run(MultiplayerGame* game){
	this->game = game;
	pthread_t thread;
	pthread_t gameThread;
	pthread_t eventThread;
	pthread_attr_t        attr;
	pthread_attr_init(&attr);

	pthread_create(&eventThread,&attr,readEvents,NULL);

	/* Listen */
	if (listen(serverID, BACKLOG) == -1) {
		Logs::logErrorMessage("Servidor: Error al poner al servidor en modo de recepcion de conexiones");
		exit(1);
	}


	pthread_create(&gameThread,&attr,runGameBackEnd,(void*)game);


	/* Main loop */
	while (1) {
		unsigned int size = sizeof(struct sockaddr_in);
		struct sockaddr_in their_addr;
		int newsock = accept(serverID, (struct sockaddr*)&their_addr, &size);
		if (newsock == -1) {
			Logs::logErrorMessage("Servidor: El servidor no ha podido aceptar la conexion");
		}
		else {
			ThreadParameter* tp = (ThreadParameter*) malloc(sizeof(ThreadParameter));
			tp->clientID = newsock;
			tp->server = this;
			if (pthread_create(&thread, &attr, handle,(void*)tp ) != 0) {
				Logs::logErrorMessage("Servidor: Error al inicializar handle thread");
			}

		}


	}
}


/* *****************  FUNCIONES DE ENVIO DE ARCHIVOS *************** */

void Server::sendMap(string mapfile,int sockID){

	std::ifstream map;
	map.open(mapfile.c_str());
	if (!map.is_open()){
		Logs::logErrorMessage("Servidor: No se ha podido cargar el mapa");
		exit(1);
	}

	string line;

	printf("Sending map...\n");
	while (map.good()){
		std::getline(map,line);
		send(sockID,line.c_str(),READING_SIZE,0);
	}
	char end = EOF;
	send(sockID,&end,READING_SIZE,0);
	printf("Map Sended.\n");

	map.close();
}

void Server::sendFiles(std::vector<std::string> wBase, std::vector<std::string> woBase, int sockID) {

	ComunicationUtils::sendNumber(sockID, wBase.size());

	for( unsigned i = 0 ; i < wBase.size() ; i++ ) {

		sendFile(wBase[i], woBase[i], sockID);
	}

}

void Server::sendFile(string fileOrigin, string fileDest, int sockID) {

	// Get Picture Size
	FILE *picture;
	picture = fopen(fileOrigin.c_str(), "r");
	int size;
	fseek(picture, 0, SEEK_END);
	size = ftell(picture);
	fseek(picture, 0, SEEK_SET);

	// Send Picture Size
	ComunicationUtils::sendNumber(sockID,size);
	cout << "Sending file of size: "<<size<<endl;

	ComunicationUtils::sendString(sockID,fileDest);
	cout << "Filename: "<<fileDest<<endl;

	// Send Picture as Byte Array
	char buffer[READING_SIZE];
	cout << "Starting to send..."<<endl;
	int sentSize=0;
	while (!feof(picture)){
		memset(buffer,-1,READING_SIZE);
		fread(buffer,sizeof(char),READING_SIZE,picture);
		int sendSize = 0;
		while (sendSize != READING_SIZE){
			sendSize = send(sockID,buffer,READING_SIZE,0);
		}
		sentSize+=sendSize;

	}
	cout << "Sent size: "<<sentSize<<endl;
	fclose(picture);

}

std::vector<std::string> Server::listFilesInDirectory(std::string directory) {

	std::vector<std::string> newVector;

	std::vector<std::string> withBase = this->listFilesInDirectoryWithBase(
			directory);

	// Tamaño del dir mas /
	int length = directory.size() + 1;

	std::ofstream outputFile("testResources");

	for (unsigned i = 0; i < withBase.size(); i++) {

		newVector.push_back(withBase[i].substr(length));

		outputFile << newVector[i] << std::endl;

	}

	return newVector;

}

std::vector<std::string> Server::listFilesInDirectoryWithBase(
		std::string directory) {

	std::vector<std::string> listOfFiles;

	struct dirent *de = NULL;
	DIR *dir = NULL;
	DIR *checkDir = NULL;

	dir = opendir(directory.c_str());
	if (dir == NULL) {
		Logs::logErrorMessage(
				std::string("No se pudo abrir el directorio" + directory));
		return listOfFiles;
	}

	while ((de = readdir(dir))) {

		if (string(".").compare(de->d_name) == 0)
			continue;
		if (string("..").compare(de->d_name) == 0)
			continue;

		checkDir = opendir(string(directory + "/" + de->d_name).c_str());
		if (checkDir == NULL) {
			listOfFiles.push_back(directory + "/" + string(de->d_name));
		} else {
			std::vector<std::string> auxVector = listFilesInDirectoryWithBase(
					string(directory + "/" + de->d_name));
			for (unsigned i = 0; i < auxVector.size(); i++) {
				listOfFiles.push_back(auxVector[i]);
			}
			closedir(checkDir);
		}
	}

	return listOfFiles;

}


/* *****************  RECEPCION DE UN NUEVO JUGADOR **************** */

PlayerInfo* Server::recieveNewPlayer(int clientSocket) {
	return ComunicationUtils::recvPlayerInfo(clientSocket);
}

void Server::sendAproval(int clientSocket, int result) {
	ComunicationUtils::sendNumber(clientSocket, result);
}

int Server::isNameAbilivable(string playerName) {
	if (conectedPlayers.count(playerName) > 0)
		return ERROR;
	return OK;
}

string Server::getAbilivableName(string playerName) {

	int i = 0;
	string newName = playerName;

	while (conectedPlayers.count(newName) > 0) {
		i++;
		stringstream indexstream;
		indexstream << i;
		newName = playerName + indexstream.str();
	}

	return newName;

}

void Server::sendNewName(int clientSocket, string newName) {
	ComunicationUtils::sendString(clientSocket, newName);
}

int Server::addPlayerToGame(int clientSocket, PlayerInfo* info) {

	string playerName = info->getPlayer()->getName();

	if (disconectedPlayers.count(playerName) > 0) {
		return reconectPlayer(clientSocket, playerName, info);
	}

	game->addNewPlayer(info->getPlayer(), info->getInitCoordinates());

	gamePlayers[clientSocket] = info;
	conectedPlayers[playerName] = clientSocket;

	updates[playerName] = vector<PlayerUpdate*>();

	return 0;

}

int Server::reconectPlayer(int clientSocket, string playerName,
		PlayerInfo* info) {

	disconectedPlayers.erase(playerName);

	vector<PlayerEvent*> conectEvent;
	conectEvent.push_back(new PlayerEvent(EVENT_CONECT));
	game->addEventsToHandle(playerName, conectEvent);

	gamePlayers.insert(pair<int, PlayerInfo*>(clientSocket, info));
	conectedPlayers.insert(pair<string, int>(playerName, clientSocket));

	updates[playerName] = vector<PlayerUpdate*>();

	return 0;
}

/* *********************** SERVER MAIN LOOP ************************ */

bool Server::exchangeAliveSignals(int clientSocket) {
	string signal = ComunicationUtils::recvString(clientSocket);
	if (signal.compare(ALIVE_SIGNAL) == 0) {
		ComunicationUtils::sendString(clientSocket, ALIVE_SIGNAL);
		return true;
	}

	return false;

}

void Server::sendNewPlayers(int clientSocket, map<int, string> *sended) {

	// 1ro envio la cantidad de players que voy a mandar
	int n = gamePlayers.size() - sended->size();

	ComunicationUtils::sendNumber(clientSocket, n);

	if (n == 0)
		return;

	for (map<int, PlayerInfo*>::iterator it = gamePlayers.begin();
			it != gamePlayers.end(); ++it) {

		// SI NO HA SIDO ENVIADO, LO ENVIO
		if (sended->count(it->first) == 0) {
			PlayerInfo* info = it->second;
			ComunicationUtils::sendPlayerInfo(clientSocket, info);
			(*sended)[it->first] = it->second->getPlayer()->getName();
		}

	}

}

vector<PlayerEvent*> Server::recvEvents(int clientSocket) {

	vector<PlayerEvent*> events;

	// 1ro recibo la cantidad de cambios que se enviaran
	int n = ComunicationUtils::recvNumber(clientSocket);

	// No hubo cambios
	if (n <= 0)
		return events;

	// Recibo cada uno de los cambios
	for (int i = 0; i < n; i++) {
		PlayerEvent* event = ComunicationUtils::recvPlayerEvent(clientSocket);
		if (event != NULL)
			events.push_back(event);
	}

	return events;

}

void Server::getPlayersUpdates() {

	for (map<string, int>::iterator it = conectedPlayers.begin();
			it != conectedPlayers.end(); ++it) {

		updates[it->first] = game->getPlayersUpdates();

	}

}

void Server::sendPlayersUpdates(int clientSocket, string playerName) {

	int size = updates[playerName].size();
	// Mando la cantidad de actualizaciones
	ComunicationUtils::sendNumber(clientSocket, updates[playerName].size());

	for (int i = 0; i < size; i++) {

		// Envio la actualizacion
		ComunicationUtils::sendPlayerUpdate(clientSocket,
				updates[playerName][i]);
		delete updates[playerName][i];

	}

	updates[playerName].clear();

}

/* ************************** CLOSE SERVER ************************* */

void Server::disconectPlayer(int clientSocket, string playerName) {

	vector<PlayerEvent*> disconectEvent;
	disconectEvent.push_back(new PlayerEvent(EVENT_DISCONECT));
	game->addEventsToHandle(playerName, disconectEvent);
	gamePlayers.erase(clientSocket);
	conectedPlayers.erase(playerName);
	disconectedPlayers.insert(pair<string, int>(playerName, clientSocket));

}

/* ************************ SERVER GETTERS ************************* */

MultiplayerGame* Server::getGame() {
	return game;
}

ChatServer* Server::getChat() {
	return this->chat;
}

/* *********************** SERVER DESTRUCTOR ********************** */

Server::~Server() {
	close(serverID);
}


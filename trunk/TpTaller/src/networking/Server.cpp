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

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

#include <SDL/SDL.h>

#include <networking/Server.h>
#include <networking/ComunicationUtils.h>
#include <networking/PlayerInfo.h>

// TODO: LEER ESTO DE UN YAML
#define BACKLOG     10  /* Passed to listen() */
#define MAPFILE "./mapfile.yaml"

#define READING_SIZE 4096
#define EXTRA_SIZE 1

using namespace std;

pthread_mutex_t changes_mutex;
pthread_mutex_t playerInfo_mutex;


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
		perror("getaddrinfo");
	}

	/* Create the socket */
	serverID = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (serverID == -1) {
		perror("sock");
	}

	/* Enable the socket to reuse the address */
	if (setsockopt(serverID, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
		perror("setsockopt");
	}

	/* Bind to the address */
	if (bind(serverID, res->ai_addr, res->ai_addrlen) == -1) {
		perror("bind");
	}

	freeaddrinfo(res);
	pthread_mutex_init(&changes_mutex,NULL);

	SDL_Init(SDL_INIT_JOYSTICK);

}

void Server::sendAproval(int clientSocket, int result){
	ComunicationUtils::sendNumber(clientSocket,result);
}

int Server::isNameAbilivable(string playerName){

	if (playerNames.count(playerName) > 0) return -1;

	return 0;
}

string Server::getAbilivableName(string playerName){

	int i = 0;
	string newName = playerName;

	while (playerNames.count(newName) > 0){
		i++;
		stringstream indexstream;
		indexstream << i;
		newName = playerName + indexstream.str();
	}

	return newName;

}

void Server::sendNewName(int clientSocket, string newName){
	ComunicationUtils::sendString(clientSocket, newName);
}

MultiplayerGame* Server::getGame(){
	return game;
}

void Server::getPlayersUpdates(){

	for (map<string,int>::iterator it = playerNames.begin() ; it != playerNames.end() ; ++it) {

		updates[it->first] = game->getPlayersUpdates();

//		if (update != NULL) delete update;
	}

}

void* handle(void* par){

	/* send(), recv(), close() */
		ThreadParameter* parameter = (ThreadParameter*) par;
		int clientSocket = parameter->clientID;
		Server* server = parameter->server;
		MultiplayerGame* game = server->getGame();

		//Lo primero que hago es mandar el mapa.
		//server->sendMap(std::string(MAPFILE),clientSocket);

		// Manda las imagenes y sonidos necesarios que se utilizaran.
		//TODO : sendResources(sockID);

		map<int,string> sended;



		PlayerInfo* info = server->recieveNewPlayer(clientSocket);
		string playerName = info->getPlayer()->getName();

		int result = server->isNameAbilivable(playerName);
		server->sendAproval(clientSocket,result);
		if (result != 0){
			playerName = server->getAbilivableName(playerName);
			server->sendNewName(clientSocket, playerName);
			info->setName(playerName);
			info->getPlayer()->setName(playerName);
		}

		server->addPlayerToGame(clientSocket,info);
		sended.insert( pair<int,string>(clientSocket,playerName) );

		cout << playerName << " has conected.. " << endl;

		bool playing = true;

		while (playing){

			/* Aca se hace todo el manejo de actualizaciones.
			 * Hay que mandar y recibir las actualizaciones de el resto de los jugadores
			 */
			server->sendNewPlayers(clientSocket, &sended);
			vector<PlayerEvent*> events = server->recvEvents(clientSocket);
			if (!events.empty()) game->addEventsToHandle(playerName,events);

			server->getPlayersUpdates();
			server->sendPlayersUpdates(clientSocket, playerName);

		}
		return NULL;

}

void* runGameBackEnd(void* parameter){
	MultiplayerGame* game = (MultiplayerGame*) parameter;
	game->run();
	return NULL;
}

void* readEvents(void* par ){

	while (true){

		SDL_Event event;
		while (SDL_PollEvent(&event)){

			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q){
				exit(0);
			}
		}

	}

	return NULL;

}


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
		perror("listen");
	}


	pthread_create(&gameThread,&attr,runGameBackEnd,(void*)game);


	/* Main loop */
	while (1) {
		unsigned int size = sizeof(struct sockaddr_in);
		struct sockaddr_in their_addr;
		int newsock = accept(serverID, (struct sockaddr*)&their_addr, &size);
		if (newsock == -1) {
			perror("accept");
		}
		else {
			ThreadParameter* tp = (ThreadParameter*) malloc(sizeof(ThreadParameter));
			tp->clientID = newsock;
			tp->server = this;
			if (pthread_create(&thread, &attr, handle,(void*)tp ) != 0) {
				fprintf(stderr, "Failed to create thread\n");
			}
		}


	}
}


void Server::sendMap(string mapfile,int sockID){

	std::ifstream map;
	map.open(mapfile.c_str());
	if (!map.is_open()){
		//TODO: LOG
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

PlayerInfo* Server::recieveNewPlayer(int clientSocket){

	return ComunicationUtils::recvPlayerInfo(clientSocket);

}

int Server::addPlayerToGame(int clientSocket, PlayerInfo* info){

	if (playerNames.count(info->getPlayer()->getName()) > 0){
		return 1;
	}

	game->addNewPlayer(info->getPlayer(), info->getInitCoordinates());


	gamePlayers[clientSocket] = info;
	playerNames[info->getPlayer()->getName()] = clientSocket;
	updates[info->getPlayer()->getName()] = vector<PlayerUpdate*>();

	return 0;

}

void Server::sendNewPlayers(int clientSocket, map<int,string> *sended){

	// 1ro envio la cantidad de players que voy a mandar
	int n = gamePlayers.size() - sended->size();
	stringstream ss;

	ss << n;

	send(clientSocket,ss.str().c_str(),(10)*sizeof(char), MSG_EOR);

	if (n == 0) return;

	for (map<int,PlayerInfo*>::iterator it = gamePlayers.begin() ; it != gamePlayers.end() ; ++it){

		// SI NO HA SIDO ENVIADO, LO ENVIO
		if (sended->count(it->first) == 0){
			ComunicationUtils::sendPlayerInfo(clientSocket,it->second);
			(*sended)[it->first] = it->second->getPlayer()->getName();
		}

	}


}

vector<PlayerEvent*> Server::recvEvents(int clientSocket){

	vector<PlayerEvent*> events;

	// 1ro recibo la cantidad de cambios que se enviaran
	int n = ComunicationUtils::recvNumber(clientSocket);

	// No hubo cambios
	if (n <= 0) return events;

	// Recibo cada uno de los cambios
	for (int i = 0 ; i < n ; i++){
		PlayerEvent* event = ComunicationUtils::recvPlayerEvent(clientSocket);
		if (event != NULL) events.push_back(event);
	}

	return events;

}

void Server::sendPlayersUpdates(int clientSocket, string playerName){

	int size = updates[playerName].size();
	// Mando la cantidad de actualizaciones
	ComunicationUtils::sendNumber(clientSocket,updates[playerName].size());

	for (int i = 0 ; i < size ; i++){

		// Envio la actualizacion
		ComunicationUtils::sendPlayerUpdate(clientSocket,updates[playerName][i]);
		delete updates[playerName][i];

	}


	updates[playerName].clear();


}

Server::~Server() {
	close(serverID);
}


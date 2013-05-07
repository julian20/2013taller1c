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

void* handle(void* par){

	/* send(), recv(), close() */
		ThreadParameter* parameter = (ThreadParameter*) par;
		int clientSocket = parameter->clientID;
		Server* server = parameter->server;


		//Lo primero que hago es mandar el mapa.
		//server->sendMap(std::string(MAPFILE),clientSocket);

		// Manda las imagenes y sonidos necesarios que se utilizaran.
		//TODO : sendResources(sockID);

		map<int,string> sended;


		PlayerInfo* info = server->recieveNewPlayer(clientSocket);
		int result = server->addPlayerToGame(clientSocket,info);

		server->sendAproval(clientSocket,result);
		if (result != 0) return NULL;

		string playerName = info->getPlayer()->getName();
		sended.insert( pair<int,string>(clientSocket,playerName) );

		cout << playerName << " has conected.. " << endl;

		bool playing = true;

		while (playing){

			/* Aca se hace todo el manejo de actualizaciones.
			 * Hay que mandar y recibir las actualizaciones de el resto de los jugadores
			 */
			server->sendNewPlayers(clientSocket, &sended);
			list<PlayerEvent*> events = server->recvEvents(clientSocket);
			if (!events.empty()) server->addEventsToChanges(playerName,events);
			server->sendOthersChanges(clientSocket, playerName);

		}
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


void Server::run(){
	pthread_t thread;
	pthread_attr_t        attr;
	pthread_attr_init(&attr);

	pthread_create(&thread,&attr,readEvents,NULL);

	/* Listen */
	if (listen(serverID, BACKLOG) == -1) {
		perror("listen");
	}

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

	gamePlayers[clientSocket] = info;
	playerNames[info->getPlayer()->getName()] = clientSocket;
	pthread_mutex_lock(&changes_mutex);
	changes[info->getPlayer()->getName()] = new Changes();
	pthread_mutex_unlock(&changes_mutex);

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

list<PlayerEvent*> Server::recvEvents(int clientSocket){

	list<PlayerEvent*> events;

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

void Server::addEventsToChanges(string playerName, list<PlayerEvent*> events){

	for (map<string, Changes* >::iterator it = changes.begin() ; it != changes.end() ; ++it){
		if (it->first != playerName){
			pthread_mutex_lock(&changes_mutex);
			it->second->addChanges(playerName,events);
			pthread_mutex_unlock(&changes_mutex);
		}
	}

}

void Server::sendEvents(int clientSocket, list<PlayerEvent*> events){

	// 1ro envio la cantidad de eventos que voy a mandar
	ComunicationUtils::sendNumber(clientSocket,events.size());

	for (list<PlayerEvent*>::iterator it = events.begin() ; it != events.end() ; ++it ){
		ComunicationUtils::sendPlayerEvent(clientSocket,*it);
	}

}

void Server::sendPlayerEvents(int clientSocket, string name,list<PlayerEvent*> events){

	// Envio el nombre del jugador y luego sus eventos
	ComunicationUtils::sendString(clientSocket,name);

	sendEvents(clientSocket,events);

}

void Server::sendOthersChanges(int clientSocket, string currentPlayer){

	pthread_mutex_lock(&changes_mutex);
	map<string,list<PlayerEvent*> > others = changes[currentPlayer]->getChanges();
	pthread_mutex_unlock(&changes_mutex);

	// 1ro envio la cantidad de players que tuvieron eventos
	ComunicationUtils::sendNumber(clientSocket,others.size());

	if (others.size() <= 0) return;

	for (map<string, list<PlayerEvent*> >::iterator it = others.begin() ; it != others.end() ; ++it){
		sendPlayerEvents(clientSocket, it->first,it->second);
	}

	pthread_mutex_lock(&changes_mutex);
	delete changes[currentPlayer];
	changes[currentPlayer] = new Changes();
	pthread_mutex_unlock(&changes_mutex);

}

Server::~Server() {
	close(serverID);
}


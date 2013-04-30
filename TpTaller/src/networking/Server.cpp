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

#include <networking/Server.h>
#include <networking/PlayerInfo.h>

// TODO: LEER ESTO DE UN YAML
#define BACKLOG     10  /* Passed to listen() */
#define MAPFILE "./mapfile.yaml"

#define READING_SIZE 4096
#define EXTRA_SIZE 1

using namespace std;


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

	changes = new Changes();

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

		map<int,int> sended;
		sended.insert( pair<int,int>(clientSocket,clientSocket) );

		PlayerInfo* info = server->recieveNewPlayer(clientSocket);
		server->addPlayerToGame(clientSocket,info);

		bool playing = true;

		while (playing){

			/* Aca se hace todo el manejo de actualizaciones.
			 * Hay que mandar y recibir las actualizaciones de el resto de los jugadores
			 */


			server->sendNewPlayers(clientSocket, &sended);

			PlayerEvent* event = server->downloadEvents(clientSocket);

			if (event != NULL)
				server->addEventToChanges(clientSocket,event);

			server->sendOthersChanges(clientSocket);

		}
		return NULL;

}


void Server::run(){
	pthread_t thread;
	pthread_attr_t        attr;
	pthread_attr_init(&attr);

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
			printf("Got a connection from %s on port %d: ClientID: %d \n",
					inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port), newsock);
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
	int size = 2*sizeof(PlayerInfo) + sizeof(Player);
	PlayerInfo* info = new PlayerInfo();


	// CREATE A BUFFER OF THE RECIVED SIZE
	char buffer[size];
	stringstream ss;

	// SEND PLAYER INFO
	recv(clientSocket,buffer,size, MSG_EOR);
	// Turn the char[]into a stringstream
	ss << buffer;
	ss.str().c_str();


	// Initialize the recived PlayerInfo
	ss >> *info;

	return info;

}

void Server::addPlayerToGame(int clientSocket, PlayerInfo* info){

	gamePlayers[clientSocket] = info;

}

void Server::sendPlayerInfo(int clientSocket,PlayerInfo* info){

	stringstream ss;
	ss << *(info);

	int size = 2*sizeof(PlayerInfo) + sizeof(Player);

	// SEND PLAYER INFO
	send(clientSocket,ss.str().c_str(),size, MSG_WAITALL);

}

void Server::sendNewPlayers(int clientSocket, map<int,int> *sended){

	// 1ro envio la cantidad de players que voy a mandar
	int n = gamePlayers.size() - sended->size();
	stringstream ss;

	ss << n;

	send(clientSocket,ss.str().c_str(),(10)*sizeof(char), MSG_WAITALL);

	for (map<int,PlayerInfo*>::iterator it = gamePlayers.begin() ; it != gamePlayers.end() ; ++it){

		// SI NO HA SIDO ENVIADO, LO ENVIO
		if (sended->count(it->first) == 0){
			sendPlayerInfo(clientSocket,it->second);
			sended->insert( pair<int,int>(clientSocket,clientSocket) );
		}

	}


}

PlayerEvent* Server::downloadEvents(int clientSocket){
	return NULL;
}

void Server::addEventToChanges(int clientSocket, PlayerEvent* event){

}

void Server::sendOthersChanges(int clientSocket){

}

Server::~Server() {
	close(serverID);
}


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

	changes = new GlobalChanges();

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


		bool playing = true;

		int i = 0;

		while (playing){

			GlobalChanges* changes = server->getChanges();

			// Recibo la informacion del jugador
			PlayerInfo* info = server->recievePlayerInfo(clientSocket);
			changes->addChanges(info);

			server->sendGlobalChanges(clientSocket,changes);


			cout << " Loop " << i << endl;
			i++;
			/* Aca se hace todo el manejo de actualizaciones.
			 * Hay que mandar y recibir las actualizaciones de el resto de los jugadores
			 */

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

GlobalChanges* Server::getChanges(){
	return changes;
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



PlayerInfo* Server::recievePlayerInfo(int clientSocket){

	stringstream ss;
	string temp;
	PlayerInfo* info = new PlayerInfo();
	char buffer[READING_SIZE];
	// NULL CONTROL TODO

	// Recibo el PlayerInfo.
	recv(clientSocket,buffer,READING_SIZE,0);
	temp.assign(buffer, READING_SIZE);

	ss << temp;
	ss >> *info;

	return info;

}

void Server::sendGlobalChanges(int sockID, GlobalChanges* changes){

	stringstream ss;
	ss << *changes;

	send(sockID,ss.str().c_str(),READING_SIZE,0);

	cout << "size: " << ss.str().size()*sizeof(char)<< endl;

}

Server::~Server() {
	close(serverID);
}


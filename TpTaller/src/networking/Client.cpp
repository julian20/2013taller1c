/*
 * Client.cpp
 *
 *  Created on: 25/04/2013
 *      Author: tomas
 */

#include <networking/Client.h>
#include <model/map/TextureDefinition.h>
#include <model/map/TextureHolder.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <sstream>
#include <fstream>

#define READING_SIZE 8192
#define EXTRA_SIZE 1
#define MAPFILE "./mapita.yamp"

using namespace std;


Client::Client(string host, int port, Game* game) {
	// TODO Auto-generated constructor stub
	struct sockaddr_in hints;
	struct hostent *server;

	//Creo el nuevo socket
	clientID = socket(AF_INET, SOCK_STREAM, 0);
	if (clientID < 0)
		//TODO LOG

	// Obtengo el host del servidor
	server = gethostbyname(host.c_str());
	if (server == NULL) {
		//TODO LOG
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	// Seteo las cosas necesarias para conectarme al servidor
	memset(&hints, 0, sizeof hints);
	hints.sin_family = AF_INET;
	if(inet_pton(AF_INET, host.c_str(), &hints.sin_addr)<=0)
	{
		//TODO LOG
		printf("\n inet_pton error occured\n");
	}
	hints.sin_port = htons(port);

	// Conecto al servidor utilizando el socket creado.
	if (connect(clientID,(struct sockaddr *) &hints,sizeof(hints)) < 0)
		//error("ERROR connecting"); //TODO LOG

	// Devuelvo el socket ID.
	this->game = game;

}

void Client::downloadMap(){

	char* line = (char*) malloc(READING_SIZE*sizeof(char));
	// NULL CONTROL TODO

	line[0] = 'x';

	printf("Downloading map...\n");

	std::ofstream f;
	f.open(MAPFILE);

	//NULL CONTROL TODO

	while (line[0] != EOF){

		recv(clientID,line,READING_SIZE,0);
		if (line[0] != EOF) f << line;

	}

	f.close();

}

void Client::initPlayerInfo(PlayerView* view){
	this->info = new PlayerInfo();
	info->setWalkingImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(WALKING_MODIFIER) ));
	info->setRunningImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(RUNNING_MODIFIER) ));
	info->setIdleImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(IDLE_MODIFIER) ));
	info->setAttackImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(ATTACK_MODIFIER) ));
	info->setAnchorPixel(view->getAnchorPixel());
	info->setDelay(view->getDelay());
	info->setFPS(view->getFps());
	info->setImageDimentions(view->getImageWidth(), view->getImageHeight());
	info->setPlayer(view->getPersonaje());
	Coordinates* c = new Coordinates(view->getPersonaje()->getCoordinates().getRow(), view->getPersonaje()->getCoordinates().getCol());
	info->setInitCoordinates(c);

}

void Client::registerPlayer(){

	stringstream ss;
	ss << *(this->info);

	int size = 2*sizeof(PlayerInfo) + sizeof(Player);;

	// SEND PLAYER INFO
	send(this->clientID,ss.str().c_str(),size, MSG_EOR);


}

PlayerInfo* Client::recivePlayerInfo(){
	stringstream ss;
	string temp;
	int size = 2*sizeof(PlayerInfo) + sizeof(Player);;
	PlayerInfo* info = new PlayerInfo();

	// CREATE A BUFFER OF THE RECIVED SIZE
	char buffer[size];

	// SEND PLAYER INFO
	recv(clientID,buffer,size, MSG_EOR);
	// Turn the char[]into a stringstream
	ss << buffer;
	ss.str().c_str();

	// Initialize the recived PlayerInfo
	ss >> *info;

	return info;
}


void Client::checkNewPlayers(){

	// 1ro recibo la cantidad de players nuevos que hay
	char buf[10];
	int n = -1;
	recv(clientID,buf,10*sizeof(char), MSG_EOR);
	stringstream sstream;
	sstream << buf;

	sstream >> n;

	cout << n << endl;
	// No hay nuevos jugadores
	if (n <= 0) return;

	for (int i = 0; i < n ; i++){

		// SI NO HA SIDO ENVIADO, LO ENVIO
			PlayerInfo* info = recivePlayerInfo();
			players.insert( pair<string,Player*>(info->getPlayer()->getName(), info->getPlayer()) );
			// TODO: Creo la playerView y la registro en el game.
			// TODO: Creo un eventHandler para el player que acabo de crear
		}

}

void Client::sendEvents(){

}

Changes* Client::downloadChanges(){
	return NULL;
}

void Client::updatePlayers(Changes* changes){

}

void* transmit(void* _client){

	Client* client = (Client*) _client;

	//client->downloadMap();
	bool playing = true;

	client->registerPlayer();

	while (playing){
		client->checkNewPlayers();
		client->sendEvents();
		Changes* changes = client->downloadChanges();
		client->updatePlayers(changes);
		delete changes;
	}

	return NULL;
}


void Client::run(){

	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	if (pthread_create(&thread, &attr, transmit,(void*)this ) != 0) {
		fprintf(stderr, "Failed to create thread\n");

	}

}


Client::~Client() {
	close(clientID);
}

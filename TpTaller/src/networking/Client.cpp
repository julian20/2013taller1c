/*
 * Client.cpp
 *
 *  Created on: 25/04/2013
 *      Author: tomas
 */

#include <networking/Client.h>
#include <networking/ComunicationUtils.h>
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
	if (connect(clientID,(struct sockaddr *) &hints,sizeof(hints)) < 0);
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
	info->setName(view->getPersonaje()->getName());
	info->setWalkingImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(WALKING_MODIFIER) ));
	info->setRunningImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(RUNNING_MODIFIER) ));
	info->setIdleImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(IDLE_MODIFIER) ));
	info->setAttackImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(ATTACK_MODIFIER) ));
	info->setIdleBlockingImageSrc(view->getTextureHolder()->getTextureSrc(view->getName() + string(IDLE_BLOCKING_MODIFIER) ));
	info->setAnchorPixel(view->getAnchorPixel());
	info->setDelay(view->getDelay());
	info->setFPS(view->getFps());
	info->setImageDimentions(view->getImageWidth(), view->getImageHeight());
	info->setPlayer(view->getPersonaje());
	Coordinates* c = new Coordinates(view->getPersonaje()->getCoordinates()->getRow(), view->getPersonaje()->getCoordinates()->getCol());
	info->setInitCoordinates(c);

	this->view = view;
	this->player = view->getPersonaje();
	this->players[view->getPersonaje()->getName()] = this->player;

}

void Client::registerPlayer(){
	ComunicationUtils::sendPlayerInfo(clientID,info);
}

void Client::checkNewPlayers(){

	// 1ro recibo la cantidad de players nuevos que hay
	int n = ComunicationUtils::recvNumber(clientID);
	// No hay nuevos jugadores
	if (n <= 0) return;

	for (int i = 0; i < n ; i++){

		// SI NO HA SIDO ENVIADO, LO ENVIO
			PlayerInfo* info = ComunicationUtils::recvPlayerInfo(clientID);

			if (players.count(info->getPlayer()->getName()) > 0) return;

			players.insert( pair<string,Player*>(info->getPlayer()->getName(), info->getPlayer()) );
			cout << info->getPlayer()->getName() << " has conected..." << endl;

			// Creo la playerView y la registro en el game.
			PlayerView* view = info->createPlayerView();
			Player* player = info->getPlayer();
			game->addNewPlayer(player,view, info->getInitCoordinates());


	}

}

void Client::sendEvents(){
	list<PlayerEvent*> events = game->getEvents();

	// 1ro envio la cantidad de events que voy a mandar
	ComunicationUtils::sendNumber(clientID,events.size());

	for (list<PlayerEvent*>::iterator it = events.begin() ; it != events.end() ; ++it ){
		ComunicationUtils::sendPlayerEvent(clientID,*it);
	}

	game->cleanEvents();

}

void Client::updatePlayers(map<string,PlayerUpdate*> updates){

	for (map<string,PlayerUpdate*>::iterator it = updates.begin() ; it != updates.end() ; ++it){
		if (players[it->first]){
			players[it->first]->update(it->second);
			players[it->first]->update();
		}
		delete it->second;
	}

}

int Client::getServerAproval(){
	return ComunicationUtils::recvNumber(clientID);
}

void Client::recvNewName(){
	string newName = ComunicationUtils::recvString(clientID);
	this->player->setName(newName);
	this->view->setShowableName(newName);
	this->info->setName(newName);
}

void* transmit(void* _client){

	Client* client = (Client*) _client;

	//client->downloadMap();
	bool playing = true;

	client->registerPlayer();

	int ok = client->getServerAproval();
	if (ok != 0){
		client->recvNewName();
	}


	while (playing){
		client->checkNewPlayers();
		client->sendEvents();
		map<string,PlayerUpdate*> updates = client->recvPlayersUpdates();
		if (!updates.empty()) client->updatePlayers(updates);

	}


	return NULL;
}

map<string,PlayerUpdate*> Client::recvPlayersUpdates(){

	map<string,PlayerUpdate*> updates;

	int nUpdates = ComunicationUtils::recvNumber(clientID);
	if (nUpdates <= 0) return updates;

	for (int i = 0 ; i < nUpdates ; i++){

		PlayerUpdate* update = ComunicationUtils::recvPlayerUpdate(clientID);
		string name = update->getName();

		updates[name] = update;

	}

	return updates;

}

void Client::run(){

	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	if (pthread_create(&thread, &attr, transmit,(void*)this ) != 0) {
		fprintf(stderr, "Failed to create thread\n");

	}

}

Game* Client::getGame(){
	return game;
}


Client::~Client() {
	close(clientID);
}

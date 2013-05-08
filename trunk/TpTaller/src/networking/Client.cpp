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
	Coordinates* c = new Coordinates(view->getPersonaje()->getCoordinates().getRow(), view->getPersonaje()->getCoordinates().getCol());
	info->setInitCoordinates(c);

	this->view = view;
	this->player = view->getPersonaje();

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
			players.insert( pair<string,Player*>(info->getPlayer()->getName(), info->getPlayer()) );
			cout << info->getPlayer()->getName() << " has conected..." << endl;

			// Creo la playerView y la registro en el game.
			PlayerView* view = info->createPlayerView();
			Player* player = info->getPlayer();
			game->addNewPlayer(player,view, info->getInitCoordinates());

			//Creo un eventHandler para el player que acabo de agregar
			NetworkPlayerController* controller = new NetworkPlayerController(player,game->getMapData(), game->getMapCameraView());
			controllers.insert(pair<string,NetworkPlayerController*>(player->getName(),controller));
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

vector<PlayerEvent*> Client::recvListOfEvents(){

	vector<PlayerEvent*> events;

	int n = ComunicationUtils::recvNumber(clientID);
	if (n <= 0) return events;
	for (int i = 0 ; i < n ; i++){
		PlayerEvent* event = ComunicationUtils::recvPlayerEvent(clientID);
		events.push_back(event);
	}

	return events;

}

Changes* Client::recvOthersChanges(){

	int n = ComunicationUtils::recvNumber(clientID);

	if (n <= 0) return NULL;
	Changes* changes = new Changes();
	for (int i = 0 ; i < n ; i++){

		string player = ComunicationUtils::recvString(clientID);
		vector<PlayerEvent*> events  = recvListOfEvents();
		changes->addChanges(player,events);
	}

	return changes;
}

void Client::updatePlayers(Changes* changes){

	for (map<string,NetworkPlayerController*>::iterator it = controllers.begin() ; it != controllers.end() ; ++it){
		vector<PlayerEvent*> events = changes->getPlayerEvents(it->first);
		if (events.empty()) continue;

		it->second->handleEvents(events);
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
		Changes* changes = client->recvOthersChanges();
		if (!changes) continue;
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

Game* Client::getGame(){
	return game;
}


Client::~Client() {
	close(clientID);
}

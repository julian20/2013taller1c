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

	cout << clientID <<endl;

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
//	players[view->getPersonaje()->getName()] = view->getPersonaje();

}

void Client::sendPlayerInfo(){
	std::stringstream ss;

	ss << (*info);

	// Envio la player info.
	send(clientID,ss.str().c_str(),READING_SIZE,0);

}

void Client::updatePlayers(GlobalChanges* changes){

	vector<PlayerInfo*> others = changes->getOthersChanges(info->getPlayer()->getName());

	for (int i = 0 ; i < others.size() ; i++){
		PlayerInfo* info = others[i];
		Player* player = others[i]->getPlayer();
		string name = player->getName();
		if (players.count(player->getName()) == 0){
			// NEW PLAYER -> ADD TO GAME
			PlayerView* view = new PlayerView();
			TextureHolder* th = new TextureHolder();
			th->addTexture(new TextureDefinition(name + string(WALKING_MODIFIER),info->getWalkingImageSrc()));
			th->addTexture(new TextureDefinition(name + string(RUNNING_MODIFIER),info->getRunningImageSrc()));
			th->addTexture(new TextureDefinition(name + string(IDLE_MODIFIER),info->getIdleImageSrc()));
			th->addTexture(new TextureDefinition(name + string(ATTACK_MODIFIER),info->getAttackImageSrc()));
			view->setPersonaje(player);
			view->setTextureHolder(th);
			view->setAnchorPixel(info->getAnchorPixel());
			view->setImageHeight(info->getImageHeight());
			view->setImageWidth(info->getImageWidth());
			view->setName(name);
			view->setFps(info->getFPS());
			view->setDelay(info->getDelay());
			game->addNewPlayer(player,view,info->getInitCoordinates());
			players[player->getName()] = player;
		} else {
			// EXISTING PLAYER -> UPDATE
			*(players[player->getName()]) = *player;
			delete player;
		}

	}

}


GlobalChanges* Client::downloadGlobalChanges(){

	GlobalChanges* changes = new GlobalChanges();
	stringstream ss;
	char buffer[READING_SIZE];


	// Recibo los cambios
	recv(clientID,buffer,READING_SIZE,0);
	string temp;
	temp.assign(buffer);
	ss << temp;
	ss >> *changes;


	cout << "size: " << temp.size()*sizeof(char)<< endl;


	return changes;
}

void* transmit(void* _client){

	Client* client = (Client*) _client;

	//client->downloadMap();
	bool playing = true;

	while (playing){
		client->sendPlayerInfo();
		cout << "1" << endl;
		GlobalChanges* changes = client->downloadGlobalChanges();
		cout << "2" << endl;
		client->updatePlayers(changes);
		cout<< "3" << endl;
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

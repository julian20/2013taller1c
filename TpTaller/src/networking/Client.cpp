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
#include <model/Logs/Logs.h>
#include <view/Popup.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libgen.h>

#include <SDL/SDL.h>

#include <iostream>
#include <sstream>
#include <fstream>

#define MAPFILE "./mapita.yaml"
#define READING_SIZE 4092
#define ALIVE_SIGNAL "ALIVE"
#define OK 0
#define NAME_CHANGE 1
#define ERROR -1

#define DELAY 50

#define STARTING "start"
#define ENDED "ended"

using namespace std;

/* ***************************************************************** */
/* *************  FUNCIONES EJECUTADAS EN LOS THREADS ************** */
/* ***************************************************************** */

void* connectionChecker(void* par){

	Client* client = (Client*) par;

	client->chechServerOn();

	SDL_Delay(5000);
	return NULL;
}


void* transmit(void* _client) {

	Client* client = (Client*) _client;
	Game* game = client->getGame();
	pthread_t isAliveThread;

	// Espero que se active el juego (que comienze a correr)
	while (!game->isActive());

	client->registerPlayer();

	int ok = client->getServerAproval();
	if (ok != OK) {
		if (ok == ERROR){
			Logs::logErrorMessage("Cliente, ha ocurrido un error en la recepcion del nuevo nombre");
		}
		client->recvNewName();
		client->getChat()->assignPlayer(client->getPlayerName());
	}

	client->addLocalPlayer();

	pthread_create(&isAliveThread,NULL,connectionChecker,(void*)client );

	while (game->isActive()) {

		bool alive = client->exchangeAliveSignals();
		if (!alive) {
			Popup::popupWindow(string(" NO SE PUDO CONECTAR CON EL SERVIDOR (Se apago el servidor?)"));
			cerr << " SE HA CORTADO LA CONEXION. NO SE PUDO CONECTAR CON EL SERVIDOR  " << endl;
			game->setInactive();
			break;
		}

		client->checkNewPlayers();

		client->sendEvents();

		map<string, PlayerUpdate*> updates = client->recvPlayersUpdates();

		if (!updates.empty()){
			client->updatePlayers(updates);
		}


		client->sendChatChanges();

		vector<ChatMessage*> chatUpdates = client->recvChatUpdates();
		client->updateChat(chatUpdates);

	}

	pthread_cancel(isAliveThread);
	return NULL;
	return NULL;
}


/* ***************************************************************** */
/* *************************  CLASE CLIENT ************************* */
/* ***************************************************************** */

Client::Client(string host, int port) {
	struct sockaddr_in hints;
	struct hostent *server;

	//Creo el nuevo socket
	clientID = socket(AF_INET, SOCK_STREAM, 0);
	if (clientID < 0) {
		Logs::logErrorMessage("Cliente: El cliente no se ha podido inicializar");
		Popup::popupWindow(string("Cliente: El cliente no se ha podido inicializar"));
		exit(1);
	}

	// Obtengo el host del servidor
	server = gethostbyname(host.c_str());
	if (server == NULL) {
		Logs::logErrorMessage("Cliente: No se ha podido obtener el host del servidor");
		Popup::popupWindow(string("Cliente: No se ha podido obtener el host del servidor"));
		exit(1);
	}

	// Seteo las cosas necesarias para conectarme al servidor
	memset(&hints, 0, sizeof hints);
	hints.sin_family = AF_INET;
	if (inet_pton(AF_INET, host.c_str(), &hints.sin_addr) <= 0) {
		Logs::logErrorMessage("Cliente: Error al obtener la direccion IP del servidor");
		Popup::popupWindow(string("Cliente: Error al obtener la direccion IP del servidor"));
		exit(1);
	}
	hints.sin_port = htons(port);

	// Conecto al servidor utilizando el socket creado.
	if (connect(clientID, (struct sockaddr *) &hints, sizeof(hints)) < 0) {
		Logs::logErrorMessage("Cliente: Ha ocurrido un error conectandose al servidor");
		Popup::popupWindow(string("Cliente: Ha ocurrido un error conectandose al servidor"));
		exit(1);
	}
	this->game=NULL;
	this->info=NULL;
	this->view=NULL;
	this->player=NULL;
	this->chat= new Chat();

	thread = -1;

}
Chat* Client::getChat()
{
	return this->chat;
}
string Client::getPlayerName()
{
	return this->player->getName();
}
void Client::setGame(Game* game){
	this->game = game;
	this->game->setChat(this->chat);
}

/* ******************** CLIENT SET PLAYER INFO ********************* */

void Client::initPlayerInfo(PlayerView* view) {
	this->info = new PlayerInfo();
	info->setName(view->getPersonaje()->getName());
	info->setOriginalName(view->getName());

	info->setImages(view->getTextureHolder()->getPlayerImages(view->getName()));
	info->setAnchorPixel(view->getAnchorPixel());
	info->setDelay(view->getDelay());
	info->setFPS(view->getFps());
	info->setImageDimentions(view->getImageWidth(), view->getImageHeight());
	info->setPlayer(view->getPersonaje());
	Coordinates* c = new Coordinates(view->getPersonaje()->getCoordinates().getRow(),view->getPersonaje()->getCoordinates().getCol());
	info->setInitCoordinates(c);

	this->view = view;
	this->player = view->getPersonaje();
	this->chat->assignPlayer(this->player->getName());
}


/* **************************** CLIENT RUN ************************** */

void Client::run(){

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int a=pthread_create(&thread, &attr, transmit, (void*) this);
	if (a != 0) {
		Logs::logErrorMessage("Cliente: Error al inicializar transmit thread");
		exit(1);
	}
	this->game->run();
	pthread_cancel(thread);
}

/* *************  FUNCIONES DE RECEPCION DE ARCHIVOS *************** */

void Client::downloadFiles() {

	int size = ComunicationUtils::recvNumber(clientID);
	if (size < 0){
		Logs::logErrorMessage("Cliente: Error en la recepcion de archivos");
		return;
	}
	cout<<"Downloading "<< size<<" files\n";
	int cont=0;
	for (int i = 0; i < size; i++) {
		ComunicationUtils::downloadFile(clientID);
		cont++;
	}
	cout<<"Downloaded "<< cont<<" files\n";

}

void Client::chechServerOn(){

	while (true){

		if (timer.getTimeIntervalSinceStart() > 100000){
			Popup::popupWindow("Se ha perdido la conexion con el servidor");
			while (timer.getTimeIntervalSinceStart() > 100000){
			}
			Popup::popupWindow("Se ha restablecido la conexion");
		}
		SDL_Delay(500);
	}
}



/* *********************  ENVIO DEL NUEVO JUGADOR ****************** */

void Client::registerPlayer() {
	ComunicationUtils::sendPlayerInfo(clientID, info);
}

int Client::getServerAproval() {
	return ComunicationUtils::recvNumber(clientID);
}

void Client::recvNewName() {
	string newName = ComunicationUtils::recvString(clientID);
	if (newName == string("")){
		Popup::popupWindow("Ha ocurrido un error en la recepcion del nuevo nombre.");
		exit(1);
	}
	this->player->setName(newName);
	this->view->setShowableName(newName);
	this->info->setName(newName);
}

void Client::addLocalPlayer() {
	this->players.insert(pair<string, Player*>(player->getName(), player));
}

/* *********************** CLIENT MAIN LOOP ************************ */

bool Client::exchangeAliveSignals() {
	timer.start();
	ComunicationUtils::sendString(clientID, ALIVE_SIGNAL);
	string signal = ComunicationUtils::recvString(clientID);
	if (signal.compare(ALIVE_SIGNAL) == 0){
		return true;
	}


	return false;

}

void Client::checkNewPlayers() {

	// 1ro recibo la cantidad de players nuevos que hay
	int n = ComunicationUtils::recvNumber(clientID);
	// No hay nuevos jugadores
	if (n <= 0)
		return;

	for (int i = 0; i < n; i++) {

		PlayerInfo* info = ComunicationUtils::recvPlayerInfo(clientID);
		if (!info){
			Logs::logErrorMessage("Ciente: No se ha recibido la informacion del jugador");
			return;
		}
		string playerName = info->getPlayer()->getName();

		for (map<string, Player*>::iterator it = players.begin();
				it != players.end(); ++it) {
		}

		if (players.count(playerName) != 0)
			return;

		players.insert(
				pair<string, Player*>(info->getPlayer()->getName(),
						info->getPlayer()));
		cout << info->getPlayer()->getName() << " has conected..." << endl;

		// Creo la playerView y la registro en el game.
		PlayerView* view = info->createPlayerView();
		Player* player = info->getPlayer();
		game->addNewPlayer(player, view, info->getInitCoordinates());

	}

}

void Client::sendEvents() {
	list<PlayerEvent*> events = game->getEvents();

	// 1ro envio la cantidad de events que voy a mandar
	ComunicationUtils::sendNumber(clientID, events.size());

	for (list<PlayerEvent*>::iterator it = events.begin(); it != events.end(); ++it) {
		ComunicationUtils::sendPlayerEvent(clientID, *it);
	}

	game->cleanEvents();

}
void Client::sendChatChanges(){

	//Chat* chat = player->getChat();
	vector<ChatMessage*> mensajesEnviados = this->chat->getMessagesSend();

	// 1ro envio la cantidad de events que voy a mandar
	ComunicationUtils::sendNumber(clientID, mensajesEnviados.size());

	for(unsigned i=0; i<mensajesEnviados.size();i++)
	{
		ComunicationUtils::sendChatMessage(clientID,mensajesEnviados[i]);
	}
	this->chat->clearNewMessagesSend();
}
vector<ChatMessage*> Client::recvChatUpdates() {

	vector<ChatMessage*> updates;

	int nUpdates = ComunicationUtils::recvNumber(clientID);
	if (nUpdates <= 0)
		return updates;

	for (int i = 0; i < nUpdates; i++) {

		string msj=ComunicationUtils::recvString(clientID);
		string reciever=ComunicationUtils::recvString(clientID);
		string sender=ComunicationUtils::recvString(clientID);

		ChatMessage* update= new ChatMessage();
		update->setMSJ(msj);
		update->setReceptor(reciever);
		update->setSender(sender);
		updates.push_back(update);

	}

	return updates;

}
map<string, PlayerUpdate*> Client::recvPlayersUpdates() {

	map<string, PlayerUpdate*> updates;

	int nUpdates = ComunicationUtils::recvNumber(clientID);
	if (nUpdates <= 0)
		return updates;

	for (int i = 0; i < nUpdates; i++) {

		PlayerUpdate* update = ComunicationUtils::recvPlayerUpdate(clientID);
		string name = update->getName();
		updates[name] = update;

	}

	return updates;

}
void Client::updateChat(vector<ChatMessage*> updates)
{

	for (unsigned i=0; i<updates.size(); i++)
	{
		this->chat->Enable();

		this->chat->newMessageReceive(updates[i]);
	}

}

void Client::updatePlayers(map<string, PlayerUpdate*> updates) {

	for (map<string, PlayerUpdate*>::iterator it = updates.begin();
			it != updates.end(); ++it) {
		if (players.count(it->first) != 0) {
			players[it->first]->updateFromServer(it->second);
		}
		delete it->second;
	}

}

/* ************************ CLIENT GETTERS ************************* */

Game* Client::getGame() {
	return game;
}

/* ********************** CLIENT DESTRUCTOR ************************ */

Client::~Client() {
	close(clientID);
}

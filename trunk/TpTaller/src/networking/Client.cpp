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
#define ERROR -1

#define DELAY 50

#define STARTING "start"
#define ENDED "ended"

using namespace std;

/* ***************************************************************** */
/* *************  FUNCIONES EJECUTADAS EN LOS THREADS ************** */
/* ***************************************************************** */

void* transmit(void* _client) {

	Client* client = (Client*) _client;
	Game* game = client->getGame();

	// Espero que se active el juego (que comienze a correr)
	while (!game->isActive());

	client->registerPlayer();

	int ok = client->getServerAproval();
	if (ok != 0) {
		client->recvNewName();
	}

	client->addLocalPlayer();

	while (game->isActive()) {

		bool alive = client->exchangeAliveSignals();
		if (!alive) {
			Popup::popupWindow(string(" NO SE PUDO CONECTAR CON EL SERVIDOR (Se apagó el servidor?)"));
			cerr << " SE HA CORTADO LA CONEXION. NO SE PUDO CONECTAR CON EL SERVIDOR  " << endl;
			game->setInactive();
			break;
		}


	//	cout<<"checkea un new playerr"<<endl;
		client->checkNewPlayers();

//		cout<<"send eventes"<<endl;
		client->sendEvents();



	//	cout<<"recibe player updates"<<endl;
		map<string, PlayerUpdate*> updates = client->recvPlayersUpdates();

		//cout<<"checkea un new player"<<endl;
		if (!updates.empty()){
			client->updatePlayers(updates);
		}

		//cout<<"pregunta si cambio el chat"<<endl;
		Chat* chat=client->getChat();
		client->sendChatChanges();
		//cout<<"envia los cambios del chat"<<endl;
		vector<ChatMessage*> chatUpdates = client->recvChatUpdates();
		//cout<<"recibe los chat updates"<<endl;
		client->updateChat(chatUpdates);

		//cout<<"termino de mandar todo"<<endl;
//		map<string,ChatUpdate*> updatesChat = client->recvChatUpdates();
//		if (!updatesChat.empty()) {
//			client->updateChat(updatesChat);
//		}

	}

	// TODO: aca habria que verificar un flag seteado por el
	// exchange alive signals. Si se salio de esto porque
	// se murio la conexion hay que mostrar un mensaje, o
	// salir bien. Digo que es un flag y no siempre, porque
	// para la entrega 3 podria no estar playing y no haberse
	// roto la conexion.
	//return NULL;
}


/* ***************************************************************** */
/* *************************  CLASE CLIENT ************************* */
/* ***************************************************************** */

Client::Client(string host, int port) {
	// TODO Auto-generated constructor stub
	struct sockaddr_in hints;
	struct hostent *server;

	//Creo el nuevo socket
	clientID = socket(AF_INET, SOCK_STREAM, 0);
	if (clientID < 0) {
		Logs::logErrorMessage(
				"Cliente: El cliente no se ha podido inicializar");
		exit(1);
	}

	// Obtengo el host del servidor
	server = gethostbyname(host.c_str());
	if (server == NULL) {
		Logs::logErrorMessage(
				"Cliente: No se ha podido obtener el host del servidor");
		exit(1);
	}

	// Seteo las cosas necesarias para conectarme al servidor
	memset(&hints, 0, sizeof hints);
	hints.sin_family = AF_INET;
	if (inet_pton(AF_INET, host.c_str(), &hints.sin_addr) <= 0) {
		Logs::logErrorMessage(
				"Cliente: Error al obtener la direccion IP del servidor");
		exit(1);
	}
	hints.sin_port = htons(port);

	// Conecto al servidor utilizando el socket creado.
	if (connect(clientID, (struct sockaddr *) &hints, sizeof(hints)) < 0) {
		Logs::logErrorMessage(
				"Cliente: Ha ocurrido un error conectandose al servidor");
		exit(1);
	}
	this->game=NULL;
	this->info=NULL;
	this->view=NULL;
	this->player=NULL;
	cout<<"se crea un nuevo chat"<<endl;
	this->chat= new Chat();

}
Chat* Client::getChat()
{
	return this->chat;
}
void Client::setGame(Game* game){
	this->game = game;
	this->game->setChat(this->chat);

	//cout<<"Setea el chat al game desde el client"<<endl;
}

/* ******************** CLIENT SET PLAYER INFO ********************* */

void Client::initPlayerInfo(PlayerView* view) {
	this->info = new PlayerInfo();
	info->setName(view->getPersonaje()->getName());
	info->setWalkingImageSrc(
			view->getTextureHolder()->getTextureSrc(
					view->getName() + string(WALKING_MODIFIER)));
	info->setRunningImageSrc(
			view->getTextureHolder()->getTextureSrc(
					view->getName() + string(RUNNING_MODIFIER)));
	info->setIdleImageSrc(
			view->getTextureHolder()->getTextureSrc(
					view->getName() + string(IDLE_MODIFIER)));
	info->setAttackImageSrc(
			view->getTextureHolder()->getTextureSrc(
					view->getName() + string(ATTACK_MODIFIER)));
	info->setIdleBlockingImageSrc(
			view->getTextureHolder()->getTextureSrc(
					view->getName() + string(IDLE_BLOCKING_MODIFIER)));
	info->setAnchorPixel(view->getAnchorPixel());
	info->setDelay(view->getDelay());
	info->setFPS(view->getFps());
	info->setImageDimentions(view->getImageWidth(), view->getImageHeight());
	info->setPlayer(view->getPersonaje());
	Coordinates* c = new Coordinates(
			view->getPersonaje()->getCoordinates().getRow(),
			view->getPersonaje()->getCoordinates().getCol());
	info->setInitCoordinates(c);

	this->view = view;
	this->player = view->getPersonaje();
	this->chat->assignPlayer(this->player->getName());
	//cout<<"setea el player al cliente "<<endl;
	//this->player->setChat(this->chat);
//	cout<<"setea el chat al player"<<endl;
}


/* **************************** CLIENT RUN ************************** */

void Client::run(){

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (pthread_create(&thread, &attr, transmit, (void*) this) != 0) {
		Logs::logErrorMessage("Cliente: Error al inicializar transmit thread");
		exit(0);
	}

}

/* *************  FUNCIONES DE RECEPCION DE ARCHIVOS *************** */

void Client::downloadMap() {

	char* line = (char*) malloc(READING_SIZE * sizeof(char));
	// NULL CONTROL TODO

	line[0] = 'x';

	printf("Downloading map...\n");

	std::ofstream f;
	f.open(MAPFILE);

	//NULL CONTROL TODO

	while (line[0] != EOF) {

		recv(clientID, line, READING_SIZE, 0);
		if (line[0] != EOF)
			f << line;

	}

	f.close();

}

void Client::downloadFiles() {

	int size = ComunicationUtils::recvNumber(clientID);
	cout<<"Downloading "<< size<<" files\n";
	int cont=0;
	for (int i = 0; i < size; i++) {
		ComunicationUtils::downloadFile(clientID);
		cont++;
	}
	cout<<"Downloaded "<< cont<<" files\n";

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
	this->player->setName(newName);
	this->view->setShowableName(newName);
	this->info->setName(newName);
//	this->game->getChat()->assignPlayer(newName);
}

void Client::addLocalPlayer() {
	this->players.insert(pair<string, Player*>(player->getName(), player));
}

/* *********************** CLIENT MAIN LOOP ************************ */

bool Client::exchangeAliveSignals() {

	ComunicationUtils::sendString(clientID, ALIVE_SIGNAL);
	string signal = ComunicationUtils::recvString(clientID);
	if (signal.compare(ALIVE_SIGNAL) == 0)
		return true;

	return false;

}

void Client::checkNewPlayers() {

	// 1ro recibo la cantidad de players nuevos que hay
	int n = ComunicationUtils::recvNumber(clientID);
	// No hay nuevos jugadores
	if (n <= 0)
		return;

	for (int i = 0; i < n; i++) {

		// SI NO HA SIDO ENVIADO, LO ENVIO
		PlayerInfo* info = ComunicationUtils::recvPlayerInfo(clientID);
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

	for (list<PlayerEvent*>::iterator it = events.begin(); it != events.end();
			++it) {
		ComunicationUtils::sendPlayerEvent(clientID, *it);
	}

	game->cleanEvents();

}
void Client::sendChatChanges(){

	//Chat* chat = player->getChat();
	vector<ChatMessage*> mensajesEnviados = this->chat->getMessagesSend();

	// 1ro envio la cantidad de events que voy a mandar
	//cout<<"cliente va a mandar "<<mensajesEnviados.size()<<" mensajes."<<endl;
	ComunicationUtils::sendNumber(clientID, mensajesEnviados.size());

	for(int i=0; i<mensajesEnviados.size();i++)
	{
		ComunicationUtils::sendChatMessage(clientID,mensajesEnviados[i]);
	}
	this->chat->clearNewMessagesSend();
}
vector<ChatMessage*> Client::recvChatUpdates() {

	vector<ChatMessage*> updates;

	//cout<<"Cliente esta esperando la cantidad de datos de chat"<<endl;
	int nUpdates = ComunicationUtils::recvNumber(clientID);
	//cout<<"le vienen "<<nUpdates<<endl;
	if (nUpdates <= 0)
		return updates;

	for (int i = 0; i < nUpdates; i++) {

		//ChatMessage* update = ComunicationUtils::recvChatMessage(clientID);
		string msj=ComunicationUtils::recvString(clientID);
		string reciever=ComunicationUtils::recvString(clientID);
		string sender=ComunicationUtils::recvString(clientID);
		//string name = update->getReceiver();

		ChatMessage* update= new ChatMessage();
		update->setMSJ(msj);
		update->setReceptor(reciever);
		update->setSender(sender);
		//cout<<"el msj que le viene al cliente es "<<update->getMSJ()<< " para "<<update->getReceptor()<<" de "<<update->getSender() <<endl;
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

	for (int i=0; i<updates.size(); i++)
	{
		this->chat->Enable();
		this->chat->newMessageReceive(updates[i]);
	//	delete updates[i];
	}

}

void Client::updatePlayers(map<string, PlayerUpdate*> updates) {

	for (map<string, PlayerUpdate*>::iterator it = updates.begin();
			it != updates.end(); ++it) {
		if (players.count(it->first) != 0) {
			players[it->first]->update(it->second);
		//	players[it->first]->setChat(this->chat);
			//	players[it->first]->update();
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
	pthread_join(thread,NULL);
	close(clientID);
}

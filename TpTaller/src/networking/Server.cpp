/*
 * Server.cpp
 *
 *  Created on: 26/04/2013
 *      Author: tomas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <SDL/SDL.h>

#include <model/Logs/Logs.h>
#include <view/timer/Timer.h>
#include <networking/Server.h>
#include <networking/ComunicationUtils.h>
#include <networking/PlayerInfo.h>


#define BACKLOG     30  /* Passed to listen() */
#define READING_SIZE 4092
#define ALIVE_SIGNAL "ALIVE"

#define STARTING "start"
#define ENDED "ended"

using namespace std;

/* ******************** MUTEX FOR FILE TRANSFER ******************** */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* ************ AUXILIARY STRUCTS FOR THREAD PARAMETERS ************ */

typedef struct paramAux{
	Server* server;
	int clientID;
	string playerName;
	bool* playing;
}TimerThreadParameter;


/* ***************************************************************** */
/* *************  FUNCIONES EJECUTADAS EN LOS THREADS ************** */
/* ***************************************************************** */

void* timerChecker(void* par){

	TimerThreadParameter* parameters = (TimerThreadParameter*) par;

	Server* sv = parameters->server;
	int clientID = parameters->clientID;
	string playerName = parameters->playerName;

	sv->checkClientStatus(clientID,playerName);

	return NULL;
}

// Funcion que ejecuta al conectarse cada client
int cont =1;
void* handle(void* par) {

	signal(SIGPIPE, SIG_IGN);

	HandleThreadParameter* parameter = (HandleThreadParameter*) par;
	int clientSocket = parameter->clientID;
	Server* server = parameter->server;
	MissionManager missionManager = server->getMissionManager();


	std::vector<std::string> withBase = server->listFilesInDirectoryWithBase(
			"sendFiles");
	std::vector<std::string> withoutBase = server->listFilesInDirectory(
			"sendFiles");

	// Manda las imagenes y sonidos necesarios que se utilizaran.
	server->sendFiles(withBase, withoutBase, clientSocket);

	PlayerInfo* info = server->recieveNewPlayer(clientSocket);
	if (!info){
		Logs::logErrorMessage("No se ha recibido la informacion del jugador");
		return NULL;
	}
	string playerName = info->getPlayer()->getName();

	int result = server->isNameAbilivable(playerName);
	server->sendAproval(clientSocket, result);
	if (result != OK) {
		playerName = server->getAbilivableName(playerName);
		server->sendNewName(clientSocket, playerName);
		info->setName(playerName);
		info->getPlayer()->setName(playerName);
	}
	cont++;
	info->getPlayer()->setTeam(cont%2 +1);

	bool playing = true;
	cout << playerName << " has conected and joined team: " << info->getPlayer()->getTeam() <<endl;
	// Antes de agregarlo al juego creo el thread para chequear el estado en el que se encuentra.
	TimerThreadParameter param = {server,clientSocket,playerName, &playing};
	pthread_t timerThread;

	server->addPlayerToGame(clientSocket, info);

	pthread_create(&timerThread,NULL,timerChecker,(void*)&param);

	server->runMainLoop(clientSocket,playerName);

	pthread_cancel(timerThread);
	server->disconectPlayer(clientSocket,playerName);
	close(clientSocket);



	return NULL;

}

// Funcion que corre la logica de actualizaciones de los jugadores
void* runGameBackEnd(void* parameter) {
	MultiplayerGame* game = (MultiplayerGame*) parameter;
	game->run();
	return NULL;
}

// Funcion que lee eventos para saber cuando cerrar el servidor
void* readEvents(void* par) {

	Server* server = (Server*) par;

	while (true) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
					|| (event.type == SDL_QUIT)) {
				server->setInactive();
				delete server;
			}
		}

	}

	return NULL;

}

/* ***************************************************************** */
/* *************************  CLASE SERVER ************************* */
/* ***************************************************************** */

/* *********************** SERVER CONTRUCTOR ************************ */

Server::Server(){
	active = false;
	serverID = 0;
	game = NULL;
}

Server::Server(int port) {
	struct sockaddr_in svInfo;

	int domain = PF_INET;
	int type = SOCK_STREAM;
	int protocol = 0; //SINGLE PROTOCOL

	/* Create the socket */
	serverID = socket(domain, type, protocol);
	if (serverID == -1) {
		Logs::logErrorMessage("Servidor: Error al inicializar el servidor");
		cerr << "Servidor: Error al inicializar el servidor" << endl;
		exit(1);
	}

	svInfo.sin_family = AF_INET;
	svInfo.sin_port = htons(port);
	svInfo.sin_addr.s_addr = htons(INADDR_ANY);

	/* Bind to the address */
	if (bind(serverID, (struct sockaddr *) &svInfo, sizeof(svInfo)) < 0) {
		Logs::logErrorMessage("Servidor: Error de asignacion de direccion");
		cerr << "Servidor: Error de asignacion de direccion" << endl;
		exit(1);
	}
	active = false;

	game = NULL;
	gamePlayers.clear();
	conectedPlayers.clear();
	disconectedPlayers.clear();
	updates.clear();

}

/* **************************** SERVER RUN ************************** */

void Server::run(MultiplayerGame* game) {
	this->game = game;
	pthread_t thread;
	pthread_t gameThread;
	pthread_t eventThread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&eventThread, &attr, readEvents, (void*)this);

	/* Listen */
	if (listen(serverID, BACKLOG) == -1) {
		Logs::logErrorMessage(
				"Servidor: Error al poner al servidor en modo de recepcion de conexiones");
		cerr << "Servidor: Error al poner al servidor en modo de recepcion de conexiones" << endl;
		exit(1);
	}

	active = true;

	pthread_create(&gameThread, &attr, runGameBackEnd, (void*) game);

	/* Main loop */
	while (this->isActive()) {
		unsigned int size = sizeof(struct sockaddr_in);
		struct sockaddr_in their_addr;
		int newsock = accept(serverID, (struct sockaddr*) &their_addr, &size);
		if (newsock == -1) {
			Logs::logErrorMessage(
					"Servidor: El servidor no ha podido aceptar la conexion");
			cerr << "Servidor: El servidor no ha podido aceptar la conexion" << endl;
		} else {
			HandleThreadParameter* tp = (HandleThreadParameter*) malloc(
					sizeof(HandleThreadParameter));
			tp->clientID = newsock;
			tp->server = this;
			if (pthread_create(&thread, &attr, handle, (void*) tp) != 0) {
				Logs::logErrorMessage(
						"Servidor: Error al inicializar handle thread");
			}
			connections[newsock] = thread;

		}
	}
}

void Server::runMainLoop(int clientSocket, string playerName){
	bool playing = true;
	while (playing && isActive()) {

		playing = exchangeAliveSignals(clientSocket,playerName);
		if (!playing)
			break;

		bool gameEnd = missionManager.hasEndedGame(game->getPlayers());

		sendGameState(clientSocket, gameEnd);

		if (gameEnd) {
			break;
		}

		sendNewPlayers(clientSocket,playerName);
		sendNewMobileEntities(clientSocket,playerName);

		vector<PlayerEvent*> events = recvEvents(clientSocket);

		if (!events.empty())
			game->addEventsToHandle(playerName, events);

		getPlayersUpdates();

		sendPlayersUpdates(clientSocket, playerName);

		recvChatMessages(clientSocket);

		deliverMessages(clientSocket, playerName);

		vector<MobileEntityUpdate*> mobileEntitiesUpdates = game->getMobileEntitiesUpdates();
		sendMobileEntitiesUpdates(clientSocket,mobileEntitiesUpdates);

	}

}

/* *****************  FUNCIONES DE ENVIO DE ARCHIVOS *************** */

void Server::sendMap(string mapfile, int sockID) {

	std::ifstream map;
	map.open(mapfile.c_str());
	if (!map.is_open()) {
		Logs::logErrorMessage("Servidor: No se ha podido cargar el mapa");
		exit(1);
	}

	string line;

	printf("Sending map...\n");
	while (map.good()) {
		std::getline(map, line);
		send(sockID, line.c_str(), READING_SIZE, 0);
	}
	char end = EOF;
	send(sockID, &end, READING_SIZE, 0);
	printf("Map Sended.\n");

	map.close();
}

void Server::sendFiles(std::vector<std::string> wBase,std::vector<std::string> woBase, int sockID) {

	ComunicationUtils::sendNumber(sockID, wBase.size());
	cout<<"Sending "<< wBase.size()<<" files\n";
	int cont =0;
	for (unsigned i = 0; i < wBase.size(); i++) {
		pthread_mutex_lock(&mutex);
		ComunicationUtils::sendFile(wBase[i], woBase[i], sockID);
		cont++;
		pthread_mutex_unlock(&mutex);
	}
	cout<<"Sent "<< cont <<" files\n";

}

std::vector<std::string> Server::listFilesInDirectory(std::string directory) {

	std::vector<std::string> newVector;

	std::vector<std::string> withBase = this->listFilesInDirectoryWithBase(
			directory);

	// Tamaño del dir mas /
	int length = directory.size() + 1;

	for (unsigned i = 0; i < withBase.size(); i++) {

		newVector.push_back(withBase[i].substr(length));

	}

	return newVector;

}

std::vector<std::string> Server::listFilesInDirectoryWithBase(
		std::string directory) {

	std::vector<std::string> listOfFiles;

	struct dirent *de = NULL;
	DIR *dir = NULL;
	DIR *checkDir = NULL;

	dir = opendir(directory.c_str());
	if (dir == NULL) {
		Logs::logErrorMessage(
				std::string("No se pudo abrir el directorio" + directory));
		return listOfFiles;
	}

	while ((de = readdir(dir))) {

		if (string(".").compare(de->d_name) == 0)
			continue;
		if (string("..").compare(de->d_name) == 0)
			continue;

		checkDir = opendir(string(directory + "/" + de->d_name).c_str());
		if (checkDir == NULL) {
			listOfFiles.push_back(directory + "/" + string(de->d_name));
		} else {
			std::vector<std::string> auxVector = listFilesInDirectoryWithBase(
					string(directory + "/" + de->d_name));
			for (unsigned i = 0; i < auxVector.size(); i++) {
				listOfFiles.push_back(auxVector[i]);
			}
			closedir(checkDir);
		}
	}

	return listOfFiles;

}

/* *****************  RECEPCION DE UN NUEVO JUGADOR **************** */

PlayerInfo* Server::recieveNewPlayer(int clientSocket) {
	return ComunicationUtils::recvPlayerInfo(clientSocket);
}

void Server::sendAproval(int clientSocket, int result) {
	ComunicationUtils::sendNumber(clientSocket, result);
}

int Server::isNameAbilivable(string playerName) {
	if (conectedPlayers.count(playerName) > 0)
		return NAME_CHANGE;
	return OK;
}

string Server::getAbilivableName(string playerName) {

	int i = 0;
	string newName = playerName;

	while (conectedPlayers.count(newName) > 0) {
		i++;
		stringstream indexstream;
		indexstream << i;
		newName = playerName + indexstream.str();
	}

	return newName;

}

void Server::sendNewName(int clientSocket, string newName) {
	ComunicationUtils::sendString(clientSocket, newName);
}

int Server::addPlayerToGame(int clientSocket, PlayerInfo* info) {

	string playerName = info->getPlayer()->getName();

	if (disconectedPlayers.count(playerName) > 0) {
		return reconectPlayer(clientSocket, playerName, info);
	}

	game->addNewPlayer(info->getPlayer(), info->getInitCoordinates());

	gamePlayers[playerName] = info;
	conectedPlayers[playerName] = info->getPlayer();

	updates[playerName] = vector<PlayerUpdate*>();

	sendedPlayers[playerName][playerName] = info->getPlayer();

	return 0;

}

int Server::reconectPlayer(int clientSocket, string playerName,
		PlayerInfo* info) {

	disconectedPlayers.erase(playerName);

	vector<PlayerEvent*> conectEvent;
	conectEvent.push_back(new PlayerEvent(EVENT_CONECT));
	game->addEventsToHandle(playerName, conectEvent);

	conectedPlayers[playerName] = info->getPlayer();

	updates[playerName] = vector<PlayerUpdate*>();

	sendedPlayers[playerName][playerName] = info->getPlayer();

	return 0;
}

/* *********************** SERVER MAIN LOOP ************************ */

bool Server::exchangeAliveSignals(int clientSocket,string playerName) {
	string signal = "";
	signal = ComunicationUtils::recvString(clientSocket);
	if (signal.compare(ALIVE_SIGNAL) == 0) {
		timers[playerName].start();
		ComunicationUtils::sendString(clientSocket, ALIVE_SIGNAL);
		return true;
	}

	return false;

}

void Server::sendNewPlayers(int clientSocket,string playerName) {

	// 1ro envio la cantidad de players que voy a mandar
	int n = gamePlayers.size() - sendedPlayers[playerName].size();

	ComunicationUtils::sendNumber(clientSocket, n);

	if (n == 0)
		return;

	for (map<string, PlayerInfo*>::iterator it = gamePlayers.begin(); it != gamePlayers.end(); ++it) {
		// SI NO HA SIDO ENVIADO, LO ENVIO
		if (sendedPlayers[playerName].count(it->first) == 0) {
			PlayerInfo* info = it->second;
			ComunicationUtils::sendPlayerInfo(clientSocket, info);
			sendedPlayers[playerName][it->first] = it->second->getPlayer();
		}

	}

}

void Server::sendNewMobileEntities(int clientSocket,string playerName){
	// 1ro envio la cantidad de mobs que voy a mandar
	map<int,MobileEntityInfo*> infos = game->getMobileEntityInfo();
	int n = infos.size() - sendedMobileEntities[playerName].size();

	ComunicationUtils::sendNumber(clientSocket, n);

	for (map<int, MobileEntityInfo*>::iterator it = infos.begin(); it != infos.end(); ++it) {
		// SI NO HA SIDO ENVIADO, LO ENVIO
		if (sendedMobileEntities[playerName].count(it->first) == 0) {
			MobileEntityInfo* info = it->second;
			ComunicationUtils::sendMobileEntityInfo(clientSocket, info);
			sendedMobileEntities[playerName][it->first] = (Mob*) it->second->getEntity();
		}

	}
}

vector<PlayerEvent*> Server::recvEvents(int clientSocket) {

	vector<PlayerEvent*> events;

	// 1ro recibo la cantidad de cambios que se enviaran
	int n = ComunicationUtils::recvNumber(clientSocket);

	// No hubo cambios
	if (n <= 0)
		return events;

	// Recibo cada uno de los cambios
	for (int i = 0; i < n; i++) {
		PlayerEvent* event = ComunicationUtils::recvPlayerEvent(clientSocket);
		if (event != NULL)
			events.push_back(event);
	}

	return events;

}

void Server::getPlayersUpdates() {

	for (map<string, Player*>::iterator it = conectedPlayers.begin();
			it != conectedPlayers.end(); ++it) {

		updates[it->first] = game->getPlayersUpdates();

	}

}

void Server::sendPlayersUpdates(int clientSocket, string playerName) {

	int size = updates[playerName].size();
	// Mando la cantidad de actualizaciones
	ComunicationUtils::sendNumber(clientSocket, size);

	for (int i = 0; i < size; i++) {

		// Envio la actualizacion
		ComunicationUtils::sendPlayerUpdate(clientSocket,updates[playerName][i]);
		delete updates[playerName][i];

	}

	updates[playerName].clear();

}

void Server::sendMobileEntitiesUpdates(int clientSocket,vector<MobileEntityUpdate*> mobUpdates){
	int size = mobUpdates.size();
	// Mando la cantidad de actualizaciones
	ComunicationUtils::sendNumber(clientSocket, size);

	for (int i = 0; i < size; i++) {
		// Envio la actualizacion
		ComunicationUtils::sendMobileEntityUpdate(clientSocket,mobUpdates[i]);
		delete mobUpdates[i];
	}
}

void Server::recvChatMessages(int clientSocket){

	vector<ChatMessage*> chatMessages;

	// 1ro recibo la cantidad de cambios que se enviaran
	int n = ComunicationUtils::recvNumber(clientSocket);
	// No hubo cambios
	if (n <= 0)
		return ;

	// Recibo cada uno de los cambios
	for (int i = 0; i < n; i++) {
		ChatMessage* msj = ComunicationUtils::recvChatMessage(clientSocket);
		if (msj != NULL)
			this->messages.push_back(msj);
	}
}

void Server::deliverMessages(int clientSocket,string playerName){
	//Hay que filtrar los clientes

	vector<ChatMessage*> vecAux;
	map<int,int>  cantMapAux;

	int cant=0;

	for (unsigned i = 0 ; i < this->messages.size() ; i++)
	{
		ChatMessage* msj=this->messages[i];
		string receptor=msj->getReceptor();
		if(receptor.compare(playerName) == 0 ){

			vecAux.push_back(this->messages[i]);
			std::swap(this->messages[i], this->messages.back());
			messages.pop_back();
			cant++;

		}
	}
	ComunicationUtils::sendNumber(clientSocket, cant);
	for (int i=0; i<cant;i++) {
		ComunicationUtils::sendString(clientSocket,vecAux[i]->getMSJ());
		ComunicationUtils::sendString(clientSocket,vecAux[i]->getReceptor());
		ComunicationUtils::sendString(clientSocket,vecAux[i]->getSender());
		delete vecAux[i];

	}



}


void Server::setMessages(vector<ChatMessage*> msjs)
{
	for(unsigned i=0; i<msjs.size();i++)
	{
		this->messages.push_back(msjs[i]);
	}
}

void Server::sendGameState(int clientSocket, bool state) {

	int stateNumber = 0;

	if( state == true ) stateNumber = 1;

	ComunicationUtils::sendNumber(clientSocket, stateNumber);

}


/* ******************* CHECKING CONNECTION ************************* */

void Server::checkClientStatus(int clientID, string playerName){

	while (true){

		if (timers[playerName].getTimeIntervalSinceStart() > 1000){
			vector<PlayerEvent*> disconectEvent;
			disconectEvent.push_back(new PlayerEvent(EVENT_DISCONECT));
			game->addEventsToHandle(playerName, disconectEvent);

			// Espero a que se reestablezca la conexion
			while (timers[playerName].getTimeIntervalSinceStart() > 1000){
				SDL_Delay(1000);
			}

			vector<PlayerEvent*> conectEvent;
			conectEvent.push_back(new PlayerEvent(EVENT_CONECT));
			game->addEventsToHandle(playerName, conectEvent);
		}

		SDL_Delay(500);

	}

}

/* ************************** CLOSE SERVER ************************* */

void Server::disconectPlayer(int clientSocket, string playerName) {

	vector<PlayerEvent*> disconectEvent;
	disconectEvent.push_back(new PlayerEvent(EVENT_DISCONECT));
	game->addEventsToHandle(playerName, disconectEvent);

	conectedPlayers.erase(playerName);

	disconectedPlayers[playerName] = gamePlayers[playerName]->getPlayer();

	updates[playerName] = vector<PlayerUpdate*>();

	sendedPlayers[playerName].clear();

}

/* ***************** SERVER GETTERS & SETTERS ********************** */

MultiplayerGame* Server::getGame() {
	return game;
}

bool Server::isActive(){
	return active;
}

void Server::setActive(){
	this->active = true;
}

void Server::setInactive(){
	this->active = false;
}
map<string,Player*> Server::getPlayerConnected()
{
	return this->conectedPlayers;
}

MissionManager Server::getMissionManager() {
	return this->missionManager;
}

void Server::setMissionManager(MissionManager manager) {
	this->missionManager = manager;
}

/* *********************** SERVER DESTRUCTOR *********************** */

Server::~Server() {
	for (map<int,pthread_t>::iterator it = connections.begin() ; it != connections.end() ; ++it){
		pthread_join(it->second,NULL);
	}
	close(serverID);
	exit(0);
}


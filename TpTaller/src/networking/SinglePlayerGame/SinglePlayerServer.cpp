/*
 * SinglePlayerServer.cpp
 *
 *  Created on: 27/05/2013
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

#include <networking/SinglePlayerGame/SinglePlayerServer.h>

#define BACKLOG 2

using namespace std;

void* runSinglePlayerServer (void* par){
	SinglePlayerServer* sv = (SinglePlayerServer*) par;
	unsigned int size = sizeof(struct sockaddr_in);
	struct sockaddr_in their_addr;
	int newsock = accept(sv->getServerID(), (struct sockaddr*) &their_addr, &size);
	if (newsock == -1) {
		Logs::logErrorMessage("Servidor: El servidor no ha podido aceptar la conexion");
		cerr << "Servidor: El servidor no ha podido aceptar la conexion" << endl;
	} else {
		sv->setClientID(newsock);
		sv->handleConnection();
	}


	return NULL;
}

/* ***************************************************************** */
/* ******************  CLASE SINGLE PLAYER SERVER ****************** */
/* ***************************************************************** */

/* *********************** SERVER CONTRUCTOR ************************ */

SinglePlayerServer::SinglePlayerServer(int port) {
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
	svInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	/* Bind to the address */
	if (bind(serverID, (struct sockaddr *) &svInfo, sizeof(svInfo)) < 0) {
		Logs::logErrorMessage("Servidor: Error de asignacion de direccion");
		cerr << "Servidor: Error de asignacion de direccion" << endl;
		exit(1);
	}
	active = false;

	game = NULL;

	clientID = -1;
	gameThread = -1;
	serverThread = -1;

}


/* **************************** SERVER RUN ************************** */

void SinglePlayerServer::run(MultiplayerGame* game) {
	this->game = game;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	/* Listen */
	if (listen(serverID, BACKLOG) == -1) {
		Logs::logErrorMessage(
				"Servidor: Error al poner al servidor en modo de recepcion de conexiones");
		cerr << "Servidor: Error al poner al servidor en modo de recepcion de conexiones" << endl;
		exit(1);
	}

	active = true;

	pthread_create(&gameThread, &attr, runGameBackEnd, (void*) game);

	pthread_create(&serverThread,&attr, runSinglePlayerServer, (void*) this);
}


// Funcion que ejecuta al conectarse cada client
void SinglePlayerServer::handleConnection() {;

	signal(SIGPIPE, SIG_IGN);

	map<int, string> sent;
	PlayerInfo* info = recieveNewPlayer(clientID);
	if (!info){
		Logs::logErrorMessage("No se ha recibido la informacion del jugador");
		return;
	}
	string playerName = info->getPlayer()->getName();

	int result = isNameAbilivable(playerName);
	sendAproval(clientID, result);
	if (result != OK) {
		playerName = getAbilivableName(playerName);
		sendNewName(clientID, playerName);
		info->setName(playerName);
		info->getPlayer()->setName(playerName);
	}

	bool playing = true;

	sent.insert(pair<int, string>(clientID, playerName));
	addPlayerToGame(clientID, info);

	while (playing && isActive()) {

		playing = exchangeAliveSignals(clientID,playerName);
		if (!playing)
			break;

		sendNewPlayers(clientID,playerName);

		vector<PlayerEvent*> events = recvEvents(clientID);

		if (!events.empty())
			game->addEventsToHandle(playerName, events);

		getPlayersUpdates();

		sendPlayersUpdates(clientID, playerName);

		recvChatMessages(clientID);

		deliverMessages(clientID,playerName);

	}

	disconectPlayer(clientID,playerName);
	close(clientID);

}

int SinglePlayerServer::getServerID(){
	return serverID;
}
void SinglePlayerServer::setClientID(int clientID){
	this->clientID = clientID;
}


SinglePlayerServer::~SinglePlayerServer() {
	pthread_cancel(serverThread);
	pthread_cancel(gameThread);
	close(serverID);
}

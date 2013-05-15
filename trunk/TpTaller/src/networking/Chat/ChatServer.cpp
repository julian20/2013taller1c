/*
 * Chat.cpp
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#include <networking/Chat/ChatServer.h>


ChatServer::ChatServer() {
	// TODO Auto-generated constructor stub

}
void ChatServer::setSocketId(int socketId) {
	this->socketId = socketId;
}
void ChatServer::setMaxConnections(int mc) {
	this->maxConnections = mc;
}
void ChatServer::setGame(MultiplayerGame* game) {
	this->game = game;
}
ChatServer::~ChatServer() {
	// TODO Auto-generated destructor stub
}
/*
void* enviarMensajes(void* cliente) {
	int i, h, aux=0;

	//	ChatServer* chatServer= client->server;
	int conexiones[10]; //Harcodeado
	char content[30];
	if (fork()) {
		printf("\nEnter the data to be send type exit for stop:\n");
		scanf("%s", content);
		printf("selecione el id");
		scanf("%i", &h);
		while (strcmp(content, "exit") != 0) {

			send(conexiones[h], content, 30, 0);

			scanf("%s", content);
			printf("selecione el id");
			scanf("%i", &h);
		}
		int j;
		for (j = 0; j < aux; j++) {
			send(conexiones[j], "exit", 5, 0);
		}
	}
}*/
void ChatServer::addPlayerToChat(int clientSocket, string name)
{
		players[name] = clientSocket;
	//	cout<<"agrega al chat players a "<<name<<" con id "<<clientSocket<<endl;
}

void ChatServer::sendChatUpdates(int clientSocket, string playerName){

	int size = updates[playerName].size();
	// Mando la cantidad de actualizaciones
	ComunicationUtils::sendNumber(clientSocket,updates[playerName].size());

	for (int i = 0 ; i < size ; i++){

		// Envio la actualizacion
		ComunicationUtils::sendChatUpdate(clientSocket,updates[playerName][i]);
		delete updates[playerName][i];

	}

	updates[playerName].clear();

}
void ChatServer::getChatUpdates()
{
	for (map<string,int>::iterator it = players.begin() ; it != players.end() ; ++it)
	{
		updates[it->first] = game->getChatUpdates();
	}
}

void ChatServer::run() {
	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	struct sockaddr_in cli;
	unsigned int i;
	int newSockId, aux;
	i = sizeof(cli);

	while (1) {
		listen(this->socketId, this->maxConnections);
		newSockId = accept(this->socketId, ((struct sockaddr *) &cli), &i);
		if (newSockId == -1) {
			printf("\nCheck the description parameter\n");
			//return;
		} else {


			//if (pthread_create(&thread, &attr, enviarMensajes, (void*) cliente)
				//	!= 0) {
				//fprintf(stderr, "Failed to create thread\n");
		//	}

			printf("\nConnection accepted  numero %i !", aux);
			aux++;
		}
	}
}


/*
 * Chat.h
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#ifndef CHATSERVER_H_
#define CHATSERVER_H_

#include<netinet/in.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <networking/PlayerInfo.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <MultiplayerGame.h>
#include <model/Chat.h>


class ChatServer {
public:
	ChatServer();
	void run();
	void setSocketId(int id);
	void addPlayerToChat(int clientSocket, string nombre);
	void setMaxConnections(int mc);
	void getChatUpdates();
	void sendChatUpdates(int clientSocket, string nombre);
//	void* enviarMensajes(void* cliente);
	virtual ~ChatServer();
private:
	int socketId;
	map<string,vector<Chat*> > updates;
	MultiplayerGame* game;
	map<string,int> players;
	int maxConnections;

};


#endif /* CHATSERVER_H_ */

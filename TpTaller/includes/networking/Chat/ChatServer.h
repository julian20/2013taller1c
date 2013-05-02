/*
 * Chat.h
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#ifndef CHAT_H_
#define CHAT_H_

#include<netinet/in.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

class ChatServer {
public:
	ChatServer();
	void run();
	void setSocketId(int id);
	void setMaxConnections(int mc);
//	void* enviarMensajes(void* cliente);
	virtual ~ChatServer();
private:
	int socketId;
	int maxConnections;
};

typedef struct aux {
	ChatServer* server;
	int clientID;
} ThreadParameter;

#endif /* CHAT_H_ */

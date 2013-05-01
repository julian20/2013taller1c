/*
 * ClientChat.h
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#ifndef CLIENTCHAT_H_
#define CLIENTCHAT_H_
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
class ClientChat {
public:
	ClientChat();
	void run();
	void setSocketId(int id);
	virtual ~ClientChat();
private:
	int socketId;
};

#endif /* CLIENTCHAT_H_ */

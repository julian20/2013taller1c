/*
 * ClientChat.cpp
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#include <networking/Chat/ClientChat.h>

ClientChat::ClientChat() {
	// TODO Auto-generated constructor stub

}
void ClientChat::setSocketId(int sockId) {
	this->socketId = sockId;
}
ClientChat::~ClientChat() {
	// TODO Auto-generated destructor stub
}
void ClientChat::run() {
	int con, port, i, Res;
	char content[30];
	struct sockaddr_in cli;

	con = connect(this->socketId, (struct sockaddr*) &cli, sizeof(cli));
	//printf("Soy el cliente ",con);
	if (con == -1) {
		printf("\nConnection error");
		return;
	}

	if (fork()) {
		printf("\nEnter the data to be send type exit for stop:\n");
		scanf("%s", content);

		while (strcmp(content, "exit") != 0) {
			send(this->socketId, content, 30, 0);
			scanf("%s", content);
		}
		send(this->socketId, "exit", 5, 0);
	} else {
		i = recv(this->socketId, content, 30, 0);

		while (strcmp(content, "exit") != 0) {
			printf("\nServer: %s\n", content);
			i = recv(this->socketId, content, 30, 0);
		}
		send(this->socketId, "exit", 5, 0);
	}
}

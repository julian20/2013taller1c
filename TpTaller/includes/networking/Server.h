/*
 * Server.h
 *
 *  Created on: 26/04/2013
 *      Author: tomas
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <networking/Changes.h>
#include <networking/PlayerEvent.h>
#include <networking/PlayerInfo.h>


namespace std {



class Server {
public:
	Server(string host, int port);
	void run();

	void sendMap(string mapfile,int sockID);
	PlayerInfo* recieveNewPlayer(int clientSocket);
	void addPlayerToGame(int clientSocket, PlayerInfo* info);
	void sendNewPlayers(int clientSocket, map<int,int> *sended);
	PlayerEvent* downloadEvents(int clientSocket);
	void addEventToChanges(int clientSocket, PlayerEvent* event);
	void sendOthersChanges(int clientSocket);
	virtual ~Server();
private:

	void sendPlayerInfo(int clientSocket,PlayerInfo* info);

	int serverID;
	Changes* changes;
	map<int,PlayerInfo*> gamePlayers;
};

typedef struct aux{
	Server* server;
	int clientID;
}ThreadParameter;

} /* namespace std */
#endif /* SERVER_H_ */

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
	int isNameAbilivable(string playerName);
	string getAbilivableName(string playerName);
	void sendNewName(int clientSocket, string newName);
	int addPlayerToGame(int clientSocket, PlayerInfo* info);
	void sendAproval(int clientSocket, int result);
	void sendNewPlayers(int clientSocket, map<int,string> *sended);
	vector<PlayerEvent*> recvEvents(int clientSocket);
	void addEventsToChanges(string PlayerName, vector<PlayerEvent*> event);
	void sendOthersChanges(int clientSocket, string currentPlayer);
	virtual ~Server();
private:
	void sendPlayerEvents(int clientSocket,string name, vector<PlayerEvent*> events);
	void sendEvents(int clientSocket, vector<PlayerEvent*> events);

	int serverID;
	map<int,PlayerInfo*> gamePlayers;
	map<string,int> playerNames;
	map<string,Changes*> changes;
};

typedef struct aux{
	Server* server;
	int clientID;
}ThreadParameter;

} /* namespace std */
#endif /* SERVER_H_ */

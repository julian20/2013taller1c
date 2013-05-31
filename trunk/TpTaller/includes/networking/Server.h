/*
 * Server.h
 *
 *  Created on: 26/04/2013
 *      Author: tomas
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <networking/PlayerEvent.h>
#include <networking/PlayerInfo.h>
#include <MultiplayerGame.h>
#include <MissionManager.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <cstdlib>

#include <stdlib.h>
#include <pthread.h>

#define OK 0
#define NAME_CHANGE 1
#define ERROR -1

using namespace std;

class Server {
public:
	Server();
	Server(int port);

	void run(MultiplayerGame* game);

	void sendMap(string mapfile,int sockID);
	void sendFiles(vector<string> wBase, vector<string> woBase, int sockID);

	vector<string> listFilesInDirectory(string directory);
	vector<string> listFilesInDirectoryWithBase(string directory);

	PlayerInfo* recieveNewPlayer(int clientSocket);
	void sendAproval(int clientSocket, int result);
	int isNameAbilivable(string playerName);
	string getAbilivableName(string playerName);
	void sendNewName(int clientSocket, string newName);
	int addPlayerToGame(int clientSocket, PlayerInfo* info);

	bool exchangeAliveSignals(int clientSocket,string playerName);
	void sendNewPlayers(int clientSocket, string playerName);


	vector<PlayerEvent*> recvEvents(int clientSocket);
	void recvChatMessages(int clientSocket);

	void getPlayersUpdates();
	void sendPlayersUpdates(int clientSocket, string playerName);

	void checkClientStatus(int clientID, string playerName);
	void disconectPlayer(int clientSocket, string playerName);
	Chat* recvChat(int clientSocket);
	void deliverMessages(int clientSocket,string playerName);
	MultiplayerGame* getGame();

	void setMissionManager(MissionManager manager);
	MissionManager getMissionManager();

	map<string,Player*> getPlayerConnected();
	void setMessages(vector<ChatMessage*>);

	bool isActive();
	void setActive();
	void setInactive();

	virtual ~Server();

protected:
	int serverID;
	MultiplayerGame* game;
	MissionManager missionManager;
	bool active;
private:

	int reconectPlayer(int clientSocket, string playerName, PlayerInfo* info);


	map<string,PlayerInfo*> gamePlayers;
	map<string,Player*> conectedPlayers;
	map<string,Player*> disconectedPlayers;
	map<string,map<string,Player*> > sended;
	map<string,vector<PlayerUpdate*> > updates;
	vector<ChatMessage*> messages;

	map<int,pthread_t> connections;

	map<string,Timer> timers;

};

typedef struct aux{
	Server* server;
	int clientID;
}HandleThreadParameter;

void* handle(void* par);

void* runGameBackEnd(void* parameter);

#endif /* SERVER_H_ */

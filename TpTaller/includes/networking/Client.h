/*
 * Client.h
 *
 *  Created on: 25/04/2013
 *      Author: tomas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <networking/PlayerInfo.h>
#include <networking/NetworkPlayerController.h>
#include <networking/MobUpdate.h>
#include <model/entities/player/Player.h>
#include <view/entities/PlayerView.h>
#include <view/timer/Timer.h>
#include <Game.h>
#include <model/Chat.h>

#include <map>
using namespace std;

class Client {
public:
	Client(string host, int port);
	void initPlayerInfo(PlayerView* view);
	void run();

	void downloadMap();
	void downloadFiles();

	void registerPlayer();
	int getServerAproval();
	void recvNewName();
	void addLocalPlayer();

	bool recvGameState();

	bool exchangeAliveSignals();
	void checkNewPlayers();
	void sendEvents();
	map<string,PlayerUpdate*> recvPlayersUpdates();
	void updatePlayers(map<string,PlayerUpdate*> updates);
	vector<ChatMessage*> recvChatUpdates();
	void updateChat(vector<ChatMessage*> updates);
	vector<MobUpdate*> recvMobUpdates();
	void updateMobs(vector<MobUpdate*> mobUpdates);

	Game* getGame();
	Chat* getChat();
	string getPlayerName();
	void setGame(Game* game);

	void chechServerOn();

	virtual ~Client();
	void sendChatChanges();
private:

	void sendEvent(PlayerEvent* event);
	Chat* chat;
	int clientID;
	Game* game;
	map<string, Player*> players;
	PlayerInfo* info;
	vector<PlayerEvent*> events;

	Player* player;
	PlayerView* view;

	// TRANSMIT THREAD
	pthread_t thread;

	Timer timer;

};

#endif /* CLIENT_H_ */

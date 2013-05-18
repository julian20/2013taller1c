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
#include <model/entities/player/Player.h>
#include <view/entities/PlayerView.h>
#include <Game.h>
#include <model/Chat.h>

#include <map>

namespace std {

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

	bool exchangeAliveSignals();
	void checkNewPlayers();
	void sendEvents();
	map<string,PlayerUpdate*> recvPlayersUpdates();
	void updatePlayers(map<string,PlayerUpdate*> updates);
	vector<ChatMessage*> recvChatUpdates();
	void updateChat(vector<ChatMessage*> updates);
	Game* getGame();
	Chat* getChat();
	void setGame(Game* game);
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

};

} /* namespace std */
#endif /* CLIENT_H_ */

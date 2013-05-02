/*
 * Client.h
 *
 *  Created on: 25/04/2013
 *      Author: tomas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <networking/PlayerInfo.h>
#include <networking/Changes.h>
#include <model/entities/player/Player.h>
#include <view/entities/PlayerView.h>
#include <Game.h>

#include <map>

namespace std {

class Client {
public:
	Client(string host, int port, Game* game);
	Game* getGame();

	void initPlayerInfo(PlayerView* view);
	void run();

	void downloadMap();
	void registerPlayer();
	int getServerAproval();
	void checkNewPlayers();
	void sendEvents();
	Changes* recvOthersChanges();
	void updatePlayers(Changes* changes);

	virtual ~Client();
private:

	PlayerInfo* recivePlayerInfo();
	void sendEvent(PlayerEvent* event);
	list<PlayerEvent*> recvListOfEvents();

	int clientID;
	Game* game;
	map<string, Player*> players;
	PlayerInfo* info;
	list<PlayerEvent*> events;

};

} /* namespace std */
#endif /* CLIENT_H_ */

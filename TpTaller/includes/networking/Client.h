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

	void initPlayerInfo(PlayerView* view);
	void run();

	void downloadMap();
	void registerPlayer();
	void checkNewPlayers();
	void sendEvents();
	Changes* downloadChanges();
	void updatePlayers(Changes* changes);

	virtual ~Client();
private:

	PlayerInfo* recivePlayerInfo();

	int clientID;
	Game* game;
	map<string,Player*> players;
	PlayerInfo* info;
	list<PlayerEvent*> events;





};

} /* namespace std */
#endif /* CLIENT_H_ */

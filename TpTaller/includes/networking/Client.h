/*
 * Client.h
 *
 *  Created on: 25/04/2013
 *      Author: tomas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <networking/PlayerInfo.h>
#include <networking/GlobalChanges.h>
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
	void sendPlayerInfo();
	GlobalChanges* downloadGlobalChanges();
	void updatePlayers(GlobalChanges* changes);

	virtual ~Client();
private:
	int clientID;
	Game* game;
	map<string,Player*> players;
	PlayerInfo* info;





};

} /* namespace std */
#endif /* CLIENT_H_ */

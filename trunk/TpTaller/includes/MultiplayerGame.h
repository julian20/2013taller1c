/*
 * MultiplayerGame.h
 *
 *  Created on: 11/05/2013
 *      Author: tomas
 */

#ifndef MULTIPLAYERGAME_H_
#define MULTIPLAYERGAME_H_

#include <list>
#include <map>

#include <Game.h>
#include <model/persistence/PersistentConfiguration.h>
#include <networking/PlayerEvent.h>
#include <networking/NetworkPlayerController.h>
#include <networking/PlayerUpdate.h>
#include <view/MapView.h>

namespace std {

class MultiplayerGame {
public:
	MultiplayerGame(PersistentConfiguration* configuration);
	virtual MenuEvent run();
	virtual void addNewPlayer(Player* player, Coordinates* coordiantes);
	void addEventsToHandle(string playerName, vector<PlayerEvent*> events);
	vector<PlayerUpdate*> getPlayersUpdates();
	virtual ~MultiplayerGame();
private:

	MapView* view;

	list<PlayerEvent*> events;
	map< string, NetworkPlayerController*> controllers;

	list<Player*> players;
	void playersUpdate();

	int fps;
	int tempFps;
	int fpsUpdatingTimer;
	GameConfiguration* gameConfig;

	void applyFPS(int timer);
};

} /* namespace std */
#endif /* MULTIPLAYERGAME_H_ */

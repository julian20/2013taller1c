/*
 * NetworkPlayerController.h
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#ifndef NETWORKPLAYERCONTROLLER_H_
#define NETWORKPLAYERCONTROLLER_H_

#include <controller/PlayerController.h>
#include <model/entities/player/Player.h>
#include <model/map/MapData.h>
#include <model/entityProperties/Coordinates.h>
#include <view/MapCameraView.h>
#include <networking/PlayerEvent.h>

using namespace std;

class NetworkPlayerController : public PlayerController {
public:
	NetworkPlayerController(Player* player,MapData* map);
	void handleEvents(vector<PlayerEvent*>);
	void movePlayer(Coordinates* tileCoord);
	Player* getPlayer();
	void deliverMessage(ChatMessage* msj);
	virtual ~NetworkPlayerController();
	void toggleRunning();
private:

	void handleEvent(PlayerEvent* event);
};
#endif /* NETWORKPLAYERCONTROLLER_H_ */

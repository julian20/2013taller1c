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
#include <view/MapCameraView.h>
#include <networking/PlayerEvent.h>

namespace std {

class NetworkPlayerController {
public:
	NetworkPlayerController(Player* player,MapData* map, MapCameraView* camera);
	void handleEvents(list<PlayerEvent*>);
	virtual ~NetworkPlayerController();
private:
	PlayerController* controller;
	Player* player;

	void handleEvent(PlayerEvent* event);
};

} /* namespace std */
#endif /* NETWORKPLAYERCONTROLLER_H_ */

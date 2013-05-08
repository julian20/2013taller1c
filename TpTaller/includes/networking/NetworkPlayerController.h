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

namespace std {

class NetworkPlayerController : public PlayerController {
public:
	NetworkPlayerController(Player* player,MapData* map, MapCameraView* camera);
	void handleEvents(vector<PlayerEvent*>);
	void movePlayer(Coordinates* tileCoord);
	Player* getPlayer();
	virtual ~NetworkPlayerController();
private:


	void handleEvent(PlayerEvent* event);
};

} /* namespace std */
#endif /* NETWORKPLAYERCONTROLLER_H_ */

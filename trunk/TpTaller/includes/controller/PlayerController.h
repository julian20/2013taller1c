/*
 * PlayerController.h
 *
 *  Created on: Apr 9, 2013
 *      Author: gonchub
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include <model/map/MapData.h>
#include <model/entities/player/Player.h>
#include <controller/ChatController.h>
#include <view/MapCameraView.h>
#include <networking/PlayerEvent.h>

class PlayerController {
public:
	PlayerController();
	virtual ~PlayerController();
	void movePlayer(int x, int y);
	void setMapCameraView(MapCameraView* cameraView);
	void setMapData(MapData* mapData);
	void setPlayer(Player* personaje);
	MapCameraView* getMapCameraView();
	MapData* getMapData();
	Player* getPlayer();
	void toggleRunning();
	void playerAttack();
	void playerCancelAttack();
	void playerBlock();
	void playerCancelBlock();
	ChatController * getChatController();
	void generateEventList(bool activated);
	list<PlayerEvent*> getEventList();
	void cleanEventList();

protected:
	MapCameraView* camera;
	MapData* data;
	Player* player;
	bool listEvents;

	list<PlayerEvent*> events;
};

#endif /* PLAYERCONTROLLER_H_ */

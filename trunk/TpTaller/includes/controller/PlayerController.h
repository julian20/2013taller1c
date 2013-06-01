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
//#include <controller/ChatController.h>
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
	void playerAttackTo(Entity* entity);
	void playerAttack();
	void playerCancelAttack();
	void playerBlock();
	void playerCancelBlock();
	void generateEventList(bool activated);
	list<PlayerEvent*> getEventList();
	void cleanEventList();
	bool clickAnotherPlayer(int x, int y);
	MobileEntity* getLastPlayerTouch();

	bool playerHasclickedAnEntity(Coordinates* coor);
	bool playerHasclickedAnEntity(int x, int y);
	Entity* getEntityToCollideTo();
protected:
	MapCameraView* camera;
	MapData* data;
	Player* player;
	MobileEntity* lastTouchedPlayer;
	bool listEvents;
	Entity* entityToCollideAgainst;
	list<PlayerEvent*> events;
private:
	bool hasMoveEvent(list<PlayerEvent*> eventsList);
};

#endif /* PLAYERCONTROLLER_H_ */

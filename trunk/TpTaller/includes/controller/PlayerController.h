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
#include <view/MapCameraView.h>

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
private:
	MapCameraView* camera;
	MapData* data;
	Player* player;
};

#endif /* PLAYERCONTROLLER_H_ */

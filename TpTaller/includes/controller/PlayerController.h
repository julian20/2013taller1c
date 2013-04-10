/*
 * PlayerController.h
 *
 *  Created on: Apr 9, 2013
 *      Author: gonchub
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include <model/map/MapData.h>
#include <model/entities/personaje/Personaje.h>
#include <view/MapCameraView.h>

class PlayerController {
public:
	PlayerController();
	virtual ~PlayerController();
	void movePlayer(int x, int y);
	void setMapCameraView(MapCameraView* cameraView);
	void setMapData(MapData* mapData);
	void setPlayer(Personaje* personaje);
	MapCameraView* getMapCameraView();
	MapData* getMapData();
	Personaje* getPlayer();
private:
	MapCameraView* camera;
	MapData* data;
	Personaje* player;
};

#endif /* PLAYERCONTROLLER_H_ */

/*
 * MapCamera.h
 *
 *  Created on: 07/04/2013
 *      Author: tomas
 */

#ifndef MAPCAMERA_H_
#define MAPCAMERA_H_

#include <model/entityProperties/Position.h>

#include <SDL/SDL.h>

namespace std {

typedef enum {
	MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
} CameraMove;

class MapCameraView {
public:
	MapCameraView(Position* position, SDL_Surface* screen, int mapRows, int mapCols);

	void setLimitTiles(Position* lastTilePosY,Position* lastTilePosXDer,Position* lastTilePosXIzq);
	void setPosition(Position* position);
	Position* getPosition();
	SDL_Surface* getActiveScreen();
	void moveCamera(CameraMove move, int speed);
	virtual ~MapCameraView();
private:
	void setLimitTiles(int mapRows, int mapCols);
	void checkBoundaries();
	void checkBasicBoundaries();
	void checkAdvancedBoundaries();

	Position* camera;
	SDL_Surface* screen;

	Position lastTilePosY;
	Position lastTilePosXDer;
	Position lastTilePosXIzq;

};

} /* namespace std */
#endif /* MAPCAMERA_H_ */

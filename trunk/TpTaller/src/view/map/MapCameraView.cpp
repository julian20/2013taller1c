/*
 * MapCamera.cpp
 *
 *  Created on: 07/04/2013
 *      Author: tomas
 */

#include <view/MapCameraView.h>
#include <model/map/Tile.h>

namespace std {

MapCameraView::MapCameraView(Position* position, SDL_Surface* screen,
		int mapRows, int mapCols) {
	this->camera = position;
	this->screen = screen;
	setLimitTiles(mapRows, mapCols);
}

void MapCameraView::setPosition(Position* position) {
	int w = screen->w;
	int h = screen->h;
	position->setX(-position->getX() + w / 2);
	position->setY(-position->getY() + h / 2);
	this->camera = position;
}

Position* MapCameraView::getPosition() {
	return new Position(camera->getX(), camera->getY());
}

SDL_Surface* MapCameraView::getActiveScreen() {
	return screen;
}

void MapCameraView::moveCamera(CameraMove move, int cameraSpeed) {

	switch (move) {
	case MOVE_UP:
		camera->setY(camera->getY() + cameraSpeed);
		break;
	case MOVE_DOWN:
		camera->setY(camera->getY() - cameraSpeed);
		break;
	case MOVE_LEFT:
		camera->setX(camera->getX() + cameraSpeed);
		break;
	case MOVE_RIGHT:
		camera->setX(camera->getX() - cameraSpeed);
		break;
	}

	checkBoundaries();
}

void MapCameraView::setLimitTiles(int mapRows, int mapCols) {
	this->lastTilePosY = Tile::computePosition(mapRows, mapCols, false);
	this->lastTilePosXDer = Tile::computePosition(0, mapCols, false);
	this->lastTilePosXIzq = Tile::computePosition(mapRows, 0, false);
}

void MapCameraView::checkBoundaries() {
	checkBasicBoundaries();
	checkAdvancedBoundaries();
}

void MapCameraView::checkBasicBoundaries() {
	// Basic boundaries checking
	if (camera->getX() > -lastTilePosXIzq->getX() + screen->w / 2)
		camera->setX(-lastTilePosXIzq->getX() + screen->w / 2);

	if (camera->getX() < -lastTilePosXDer->getX() + screen->w / 2)
		camera->setX(-lastTilePosXDer->getX() + screen->w / 2);

	if (camera->getY() > screen->h / 2)
		camera->setY(screen->h / 2);

	if (camera->getY() < -lastTilePosY->getY() + screen->h / 2)
		camera->setY(-lastTilePosY->getY() + screen->h / 2);
}

void MapCameraView::checkAdvancedBoundaries() {
	// Advanced boundaries checking
	float XTopRightCondition = (float) ((screen->w / 2));
	float XBotRightCondition = (float) ((screen->w / 2 - lastTilePosY->getX()));
	float YTopRightCondition =
			(float) ((screen->h / 2 - lastTilePosXDer->getY()));
	float YTopLeftCondition =
			(float) ((screen->h / 2 - lastTilePosXIzq->getY()));

	bool cameraAtRightForTop = camera->getX() < XTopRightCondition;
	bool cameraAtRightForBot = camera->getX() < XBotRightCondition;
	bool cameraAtTopForRight = camera->getY() > YTopRightCondition;
	bool cameraAtTopForLeft = camera->getY() > YTopLeftCondition;

	if (cameraAtRightForTop && cameraAtTopForRight) {

		float m = (YTopRightCondition - (float) ((screen->h / 2)))
				/ ((float) ((-lastTilePosXDer->getX() + screen->w / 2))
						- XTopRightCondition);
		float b = (float) ((screen->h / 2)) - XTopRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval < camera->getY())
			camera->setY(eval);
	} else if (cameraAtRightForBot && !cameraAtTopForRight) {

		float m = (YTopRightCondition
				- (float) ((-lastTilePosY->getY() + screen->h / 2)))
				/ ((float) ((screen->w / 2 - lastTilePosXDer->getX()))
						- XBotRightCondition);
		float b = (float) ((-lastTilePosY->getY() + screen->h / 2))
				- XBotRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval > camera->getY())
			camera->setY(eval);
	} else if (!cameraAtRightForTop && cameraAtTopForLeft) {

		float m = (YTopLeftCondition - (float) ((screen->h / 2)))
				/ ((float) ((-lastTilePosXIzq->getX() + screen->w / 2))
						- XTopRightCondition);
		float b = (float) ((screen->h / 2)) - XTopRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval < camera->getY())
			camera->setY(eval);
	} else if (!cameraAtRightForBot && !cameraAtTopForLeft) {

		float m = (YTopLeftCondition
				- (float) ((-lastTilePosY->getY() + screen->h / 2)))
				/ ((float) ((screen->w / 2 - lastTilePosXIzq->getX()))
						- XBotRightCondition);
		float b = (float) ((-lastTilePosY->getY() + screen->h / 2))
				- XBotRightCondition * m;
		float eval = camera->getX() * m + b;
		if (eval > camera->getY())
			camera->setY(eval);
	}
}

MapCameraView::~MapCameraView() {
	delete lastTilePosXDer;
	delete lastTilePosXIzq;
	delete lastTilePosY;
}

} /* namespace std */

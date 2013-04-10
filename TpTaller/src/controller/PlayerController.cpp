/*
 * PlayerController.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: gonchub
 */

#include <controller/PlayerController.h>

PlayerController::PlayerController() {
	this->data = NULL;
	this->player = NULL;
	this->camera = NULL;
}

PlayerController::~PlayerController() {
	delete this->data;
	delete this->camera;
	delete this->player;
}

void PlayerController::movePlayer(int x, int y) {
	// Selecciona la casilla mas o menos bien, idealizandola como un cuadrado.
	// TODO: Que seleccione la casilla bien!
	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
			y - cameraPos->getY());

	if (!(coor->getCol() <= 0 || coor->getRow() < 0)
			&& !(coor->getCol() > data->GetNCols()
					|| coor->getRow() > data->GetNRows())) {
		SDL_Rect firstTile = Tile::computePosition(0, 0);
		firstTile.x = cameraPos->getX() + firstTile.x;
		firstTile.y = cameraPos->getY() + firstTile.y;

		if (player != NULL) {
			// TODO: esto no pierde memoria a lo loco?
			Tile* toTile = new Tile(
					new Coordinates(coor->getRow(), coor->getCol()));
			data->movePersonaje(player, toTile);
		}
	}

	delete coor;
	delete cameraPos;
}

void PlayerController::setMapCameraView(MapCameraView* cameraView) {
	this->camera = cameraView;
}

MapCameraView* PlayerController::getMapCameraView() {
	return this->camera;
}

void PlayerController::setPlayer(Player* personaje) {
	this->player = personaje;
}

Player* PlayerController::getPlayer() {
	return this->player;
}

void PlayerController::setMapData(MapData* mapData) {
	this->data = mapData;
}

MapData* PlayerController::getMapData() {
	return this->data;
}

//TODO - hardcoded
void PlayerController::toggleRunning() {
	if (player->isRunning())
		player->setSpeedMagnitude(3);
	else
		player->setSpeedMagnitude(5);
}

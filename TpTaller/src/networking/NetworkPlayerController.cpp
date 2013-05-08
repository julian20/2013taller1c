/*
 * NetworkPlayerController.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/NetworkPlayerController.h>

namespace std {

NetworkPlayerController::NetworkPlayerController(Player* player,MapData* map, MapCameraView* camera) {
	this->player = player;
	this->camera = camera;
	this->data = map;
}

void NetworkPlayerController::handleEvent(PlayerEvent* event){

	EventType type = event->getEventType();
	switch (type){
	case EVENT_RUNNING:
		this->toggleRunning();
		break;
	case EVENT_WALKING:
		this->toggleRunning();
		break;
	case EVENT_ATTACK:
		this->playerAttack();
		break;
	case EVENT_BLOCK:
		this->playerBlock();
		break;
	case EVENT_CANCEL_BLOCK:
		this->playerCancelBlock();
		break;
	case EVENT_MOVE:
		this->movePlayer(event->getTileCoordinates());
		break;
	}

}

void NetworkPlayerController::movePlayer(Coordinates* tileCoord){
	Position* cameraPos = this->camera->getPosition();

	if (!(tileCoord->getCol() <= 0 || tileCoord->getRow() < 0)
			&& !(tileCoord->getCol() > data->getNCols()
					|| tileCoord->getRow() > data->getNRows())) {
		SDL_Rect firstTile = Tile::computePositionTile(0, 0);
		firstTile.x = cameraPos->getX() + firstTile.x;
		firstTile.y = cameraPos->getY() + firstTile.y;

		if (player != NULL) {
			// TODO: esto no pierde memoria a lo loco?
			Tile* toTile = new Tile(
					new Coordinates(tileCoord->getRow(), tileCoord->getCol()));
			data->movePersonaje(player, toTile);
		}
	}



	delete cameraPos;
}


void NetworkPlayerController::handleEvents(vector<PlayerEvent*> events){

	for (vector<PlayerEvent*>::iterator it = events.begin() ; it != events.end() ; ++it ){
		handleEvent(*it);
	}

}

Player* NetworkPlayerController::getPlayer(){
	return this->player;
}

NetworkPlayerController::~NetworkPlayerController() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */

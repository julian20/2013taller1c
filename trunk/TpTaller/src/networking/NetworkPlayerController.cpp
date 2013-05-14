/*
 * NetworkPlayerController.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/NetworkPlayerController.h>

namespace std {

NetworkPlayerController::NetworkPlayerController(Player* player,MapData* map) {
	this->player = player;
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
	case EVENT_CANCEL_ATTACK:
		this->playerCancelAttack();
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
	case EVENT_STOP:
		this->player->stop();
	}

	player->setChange(true);

}

void NetworkPlayerController::movePlayer(Coordinates* tileCoord){

	if (!(tileCoord->getCol() <= 0 || tileCoord->getRow() < 0)
			&& !(tileCoord->getCol() > data->getNCols()
					|| tileCoord->getRow() > data->getNRows())) {

		if (player != NULL) {
			// TODO: esto no pierde memoria a lo loco?
			Tile* toTile = new Tile(
					new Coordinates(tileCoord->getRow(), tileCoord->getCol()));
			data->movePersonaje(player, toTile);
		}

	}

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

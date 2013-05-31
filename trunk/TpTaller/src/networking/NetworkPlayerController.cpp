/*
 * NetworkPlayerController.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/NetworkPlayerController.h>
using namespace std;

NetworkPlayerController::NetworkPlayerController(Player* player,MapData* map) {
	this->player = player;
	this->data = map;
}

void NetworkPlayerController::handleEvent(PlayerEvent* event){

	EventType type = event->getEventType();
	switch (type){
	case EVENT_NONE:
		break;
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
		break;
	case EVENT_DISCONECT:
		this->player->SetUnactive();
		break;
	case EVENT_CONECT:
		this->player->setActive();
		break;
	}

	player->setHasChanged(true);

}

void NetworkPlayerController::movePlayer(Coordinates* tileCoord){
	if (tileCoord->getCol() >= 0 && tileCoord->getCol() < data->getNCols() &&
		tileCoord->getRow() >= 0 && tileCoord->getRow() < data->getNRows() ) {

		if (player != NULL) {
			Tile* toTile = new Tile(
					new Coordinates(tileCoord->getRow(), tileCoord->getCol()));
			data->movePlayer(player, toTile);
			delete toTile;
		}

	}

}


void NetworkPlayerController::handleEvents(vector<PlayerEvent*> events){

	for (vector<PlayerEvent*>::iterator it = events.begin() ; it != events.end() ; ++it ){
		handleEvent(*it);
		delete *it;
	}

	events.clear();

}

Player* NetworkPlayerController::getPlayer(){
	return this->player;
}

NetworkPlayerController::~NetworkPlayerController() {
	// TODO Auto-generated destructor stub
}

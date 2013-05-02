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
	this->controller = new PlayerController();
	this->controller->setPlayer(this->player);
	this->controller->setMapData(map);
	this->controller->setMapCameraView(camera);
}

void NetworkPlayerController::handleEvent(PlayerEvent* event){

	EventType type = event->getEventType();
	switch (type){
	case EVENT_RUNNING:
		controller->toggleRunning();
		break;
	case EVENT_WALKING:
		controller->toggleRunning();
		break;
	case EVENT_ATTACK:
		controller->playerAttack();
		break;
	case EVENT_BLOCK:
		controller->playerBlock();
		break;
	case EVENT_CANCEL_BLOCK:
		controller->playerCancelBlock();
		break;
	case EVENT_MOVE:
		controller->movePlayer((int) event->getMousePos()->getX(),(int) event->getMousePos()->getY());
	}

}

void NetworkPlayerController::handleEvents(list<PlayerEvent*> events){

	for (list<PlayerEvent*>::iterator it = events.begin() ; it != events.end() ; ++it ){
		handleEvent(*it);
	}

}

NetworkPlayerController::~NetworkPlayerController() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */

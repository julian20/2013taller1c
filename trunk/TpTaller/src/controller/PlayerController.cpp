/*
 * PlayerController.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: gonchub
 */

#include <controller/PlayerController.h>
#include <stdio.h>
PlayerController::PlayerController() {
	this->data = NULL;
	this->player = NULL;
	this->camera = NULL;
	this->listEvents = false;
}

PlayerController::~PlayerController() {
	delete this->data;
	delete this->camera;
	delete this->player;
}
/*ChatController* PlayerController::getChatController()
{
//	Player* miPlayer=this->player;
	return (player->getChat())->getChatController();
}*/
string PlayerController::getLastPlayerTouch()
{
	return this->lastPlayerTouch;
}
bool PlayerController::clickAnotherPlayer(int x ,int y)
{
	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
					y - cameraPos->getY());
	TileData* tileData=this->data->getTileData(coor->getRow(),coor->getCol());
	if(!tileData) return false;
	MobileEntity* player=tileData->getPersonaje();
	if(player)
	{
		this->lastPlayerTouch=player->getName();
		return true;
	}
	else return false;
}
void PlayerController::movePlayer(int x, int y) {
	// Selecciona la casilla mas o menos bien, idealizandola como un cuadrado.
	// TODO: Que seleccione la casilla bien!
	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
				y - cameraPos->getY());

	if (listEvents){
		events.push_back(new PlayerEvent(EVENT_MOVE,*coor));
		if (! player->getCurrentPos()->isEqual(player->getEndPos())){
			return;
		}
		return;
	}

	if (coor->getCol() >= 0 && coor->getCol() < data->getNCols() &&
	    coor->getRow() >= 0 && coor->getRow() < data->getNRows() ) {
		SDL_Rect firstTile = Tile::computePositionTile(0, 0);
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
	if (player->isRunning()){
		if (listEvents){
			events.push_back(new PlayerEvent(EVENT_WALKING));
			return;
		}
		player->setSpeedMagnitude(player->getSpeed()->getMagnitude() / 2);

	}else {
		if (listEvents) {
			events.push_back(new PlayerEvent(EVENT_RUNNING));
			return;
		}
		player->setSpeedMagnitude(player->getSpeed()->getMagnitude() * 2);
	}
}

void PlayerController::playerAttack() {
	if (listEvents){
		events.push_back(new PlayerEvent(EVENT_ATTACK));
		events.push_back(new PlayerEvent(EVENT_STOP));
		return;
	}
	player->attack();
}

void PlayerController::playerCancelAttack(){
	if (listEvents){
		events.push_back(new PlayerEvent(EVENT_CANCEL_ATTACK));
		return;
	}
	player->cancelAttack();
}

void PlayerController::playerBlock() {
	if (listEvents){
		events.push_back(new PlayerEvent(EVENT_BLOCK));
		events.push_back(new PlayerEvent(EVENT_STOP));
		return;
	}
	player->block();

}

void PlayerController::playerCancelBlock() {
	if (listEvents){
		events.push_back(new PlayerEvent(EVENT_CANCEL_BLOCK));
		return;
	}
	player->cancelBlock();

}

void PlayerController::generateEventList(bool activated){
	this->listEvents = activated;
}

list<PlayerEvent*> PlayerController::getEventList(){
	return events;
}

void PlayerController::cleanEventList(){
	for (list<PlayerEvent*>::iterator it = events.begin(); it != events.end() ; ++it){
		delete *it;
	}
	events.clear();
}

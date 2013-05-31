/*
 * PlayerController.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: gonchub
 */

#include <controller/PlayerController.h>
#include <view/sound/SoundEffectHandler.h>
#include <stdio.h>
#include <iostream>

#define SI "resources/sound/player/si.ogg"
#define VALE "resources/sound/player/vale.ogg"
#define VOY "resources/sound/player/voy.ogg"
#define BATTLE "resources/sound/player/battleCry.ogg"

using namespace std;

double uniform(double a, double b) {
	return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

PlayerController::PlayerController() {
	this->data = NULL;
	this->player = NULL;
	this->camera = NULL;
	this->listEvents = false;
	SoundEffectHandler::loadSound(string("si"), SI);
	SoundEffectHandler::loadSound(string("voy"), VOY);
	SoundEffectHandler::loadSound(string("vale"), VALE);
	SoundEffectHandler::loadSound(string("battle"), BATTLE);
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
MobileEntity* PlayerController::getLastPlayerTouch() {
	return this->lastPlayerTouch;
}

bool PlayerController::clickAnotherPlayer(int x, int y) {
	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
			y - cameraPos->getY());
	TileData* tileData = this->data->getTileData(coor->getRow(),
			coor->getCol());
	if (!tileData)
		return false;
	MobileEntity* player = tileData->getPersonaje();
	if (player) {
		if (player->getClassName().compare("Player") != 0) return false;
		this->lastPlayerTouch = player;
		return true;
	} else
		return false;
}

void playSound() {
	if (!SoundEffectHandler::isSoundPlaying(string("si"))
			&& !SoundEffectHandler::isSoundPlaying(string("vale"))
			&& !SoundEffectHandler::isSoundPlaying(string("voy"))
			&& !SoundEffectHandler::isSoundPlaying(string("battle"))) {
		double rnd = uniform(0.0,1.0);
		if (rnd > 0.76)
			SoundEffectHandler::playSound(string("si"));
		else if (rnd > 0.5)
			SoundEffectHandler::playSound(string("voy"));
		else if (rnd > 0.25)
			SoundEffectHandler::playSound(string("vale"));
		else
			SoundEffectHandler::playSound(string("battle"));

	}

}
void PlayerController::movePlayer(int x, int y) {
	Position* cameraPos = this->camera->getPosition();
	playSound();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
			y - cameraPos->getY());

	if (listEvents) {
		events.push_back(new PlayerEvent(EVENT_MOVE, *coor));
		if (!player->getCurrentPos()->isEqual(player->getEndPos())) {
			return;
		}
		return;
	}

	if (coor->getCol() >= 0 && coor->getCol() < data->getNCols()
			&& coor->getRow() >= 0 && coor->getRow() < data->getNRows()) {
		SDL_Rect firstTile = Tile::computePositionTile(0, 0);
		firstTile.x = cameraPos->getX() + firstTile.x;
		firstTile.y = cameraPos->getY() + firstTile.y;

		if (player != NULL) {
			Tile* toTile = new Tile(
					new Coordinates(coor->getRow(), coor->getCol()));
			data->movePlayer(player, toTile);
			delete toTile;
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
	if (player->isRunning()) {
		if (listEvents) {
			events.push_back(new PlayerEvent(EVENT_WALKING));
			return;
		}
		player->setSpeedMagnitude(player->getSpeed()->getMagnitude() / 2);

	} else {
		if (listEvents) {
			events.push_back(new PlayerEvent(EVENT_RUNNING));
			return;
		}
		player->setSpeedMagnitude(player->getSpeed()->getMagnitude() * 2);
	}
}

void PlayerController::playerAttackTo(Entity* entity) {
	player->attackTo(entity);
}

void PlayerController::playerAttack() {
	if (listEvents) {
		events.push_back(new PlayerEvent(EVENT_ATTACK));
		events.push_back(new PlayerEvent(EVENT_STOP));
		return;
	}
	player->attack();
}

void PlayerController::playerCancelAttack() {
	if (listEvents) {
		events.push_back(new PlayerEvent(EVENT_CANCEL_ATTACK));
		return;
	}
	player->cancelAttack();
}

void PlayerController::playerBlock() {
	if (listEvents) {
		events.push_back(new PlayerEvent(EVENT_BLOCK));
		events.push_back(new PlayerEvent(EVENT_STOP));
		return;
	}
	player->block();

}

void PlayerController::playerCancelBlock() {
	if (listEvents) {
		events.push_back(new PlayerEvent(EVENT_CANCEL_BLOCK));
		return;
	}
	player->cancelBlock();

}

void PlayerController::generateEventList(bool activated) {
	this->listEvents = activated;
}

list<PlayerEvent*> PlayerController::getEventList() {
	PlayerEvent* playerEvent = player->getPlayerEvent();
	if ( playerEvent != NULL ) events.push_back(playerEvent);

	return events;
}

void PlayerController::cleanEventList() {
	for (list<PlayerEvent*>::iterator it = events.begin(); it != events.end();
			++it) {
		delete *it;
	}
	events.clear();
}

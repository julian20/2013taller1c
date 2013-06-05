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

double uniform2(double a, double b) {
	return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

PlayerController::PlayerController() {
	this->data = NULL;
	this->player = NULL;
	this->camera = NULL;
	this->listEvents = false;
	entityToCollideAgainst = NULL;
	lastTouchedPlayer = NULL;
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

MobileEntity* PlayerController::getLastPlayerTouch() {
	return this->lastTouchedPlayer;
}

bool PlayerController::clickAnotherPlayer(int x, int y) {
	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
			y - cameraPos->getY());
	TileData* tileData = this->data->getTileData(coor->getRow(),
			coor->getCol());
	if (!tileData)
		return false;
	MobileEntity* player = tileData->getMobileEntity();
	if (player) {
		if (player->getClassName().compare("Player") != 0)
			return false;
		this->lastTouchedPlayer = player;
		return true;
	} else
		return false;
}

bool PlayerController::playerHasclickedAnEntity(Coordinates* coor) {

	TileData* tileData = this->data->getTileData(coor->getRow(),
			coor->getCol());
	if (!tileData)
		return false;
	Entity* entity = tileData->getMobileEntity();
	if (entity) {
		if (entity->getTeam() != player->getTeam()) {
			this->entityToCollideAgainst = entity;
			return true;
		}

	} else {
		entity = tileData->getNextEntity();
		if (entity) {
			if (entity->getTeam() != player->getTeam()) {
				this->entityToCollideAgainst = entity;
				return true;
			}
		}
	}
	return false;
}

bool PlayerController::playerHasclickedAnEntity(int x, int y) {
	Position* cameraPos = this->camera->getPosition();

	Coordinates* coor = Tile::getTileCoordinates(x - cameraPos->getX(),
			y - cameraPos->getY());
	return playerHasclickedAnEntity(coor);
}

Entity* PlayerController::getEntityToCollideTo() {
	return entityToCollideAgainst;
}

void playSound() {
	if (!SoundEffectHandler::isSoundPlaying(string("si"))
			&& !SoundEffectHandler::isSoundPlaying(string("vale"))
			&& !SoundEffectHandler::isSoundPlaying(string("voy"))
			&& !SoundEffectHandler::isSoundPlaying(string("battle"))) {
		double rnd = uniform2(0.0, 1.0);
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
		playerCancelAttack();
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
	} else {
		if (listEvents) {
			events.push_back(new PlayerEvent(EVENT_RUNNING));
			return;
		}
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

bool PlayerController::hasMoveEvent(list<PlayerEvent*> eventsList) {
	list<PlayerEvent*>::const_iterator iter;
	for (iter = eventsList.begin(); iter != eventsList.end(); ++iter) {
		PlayerEvent* event = *iter;

		if (event->getEventType() == EVENT_MOVE)
			return true;
	}

	return false;
}

list<PlayerEvent*> PlayerController::getEventList() {
	list<PlayerEvent*> playerEvents = player->getPlayerEvents();
	if (playerEvents.size() != 0)
		for (list<PlayerEvent*>::iterator it = playerEvents.begin();
				it != playerEvents.end(); ++it)
			events.push_back(*it);
	return events;
}

void PlayerController::cleanEventList() {
	for (list<PlayerEvent*>::iterator it = events.begin(); it != events.end();
			++it) {
		delete *it;
	}
	events.clear();
}

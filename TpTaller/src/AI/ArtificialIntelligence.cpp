/*
 * ArtificialIntelligence.cpp
 *
 *  Created on: 27/05/2013
 *      Author: damian
 */

#include <AI/ArtificialIntelligence.h>
#include <stdlib.h>
#define WATCHSIZE 5

ArtificialIntelligence::ArtificialIntelligence() {
	entity = NULL;
}

void ArtificialIntelligence::setMobileEntity(MobileEntity* entity) {
	this->entity = entity;
}

void ArtificialIntelligence::update(MapData* mapData) {

	if (entity->isDead())
		entity->setRemoveFromGame(true);

	if (entity->isDead() || entity->isFrozen())
		return;
	if (entity->getAttackToEntity() != NULL) {
		Tile* currentTile = entity->getTile();
		Tile* enemyTile = entity->getAttackToEntity()->getTile();

		if (mapData->distBetweenTilesInTiles(currentTile, enemyTile) < WATCHSIZE) {
			entity->cancelAttack();
			delete currentTile;
			delete enemyTile;
		}
	} else {
		if (this->isAnyEnemyClose(mapData)) {
			Entity& enemy = this->getNearestEnemy();
			entity->attackTo(&enemy);
		} else {
			entity->cancelAttack();
			this->watch(mapData);
		}
	}

}

bool ArtificialIntelligence::isAnyEnemyClose(MapData* mapData) {

	entitiesNear = mapData->getClosestEntities(entity->getCoordinates(),
			WATCHSIZE);
	list<MobileEntity*>::iterator iter;

	for (iter = entitiesNear.begin(); iter != entitiesNear.end(); ++iter) {
		MobileEntity* mob = *iter;
		if (mob->getTeam() != this->entity->getTeam() && !mob->isDead())
			return true;
	}
	return false;
}

Entity& ArtificialIntelligence::getNearestEnemy() {

	MobileEntity* mob;
	list<MobileEntity*>::iterator iter;
	for (iter = entitiesNear.begin(); iter != entitiesNear.end(); ++iter) {
		mob = *iter;
		if (mob->getTeam() != this->entity->getTeam() && !mob->isDead())
			return *mob;
	}

	return *mob;
}

void ArtificialIntelligence::watch(MapData* mapData) {

	Coordinates coor = this->entity->getCoordinates();

	Tile* tile = NULL;
	//Si no se esta moviendo o atacando busco a otro contrincante
	if (!entity->IsMoving() && !entity->isDead()) {
		int direction = rand() % 4;
		if (direction == 0) {
			tile = new Tile(
					new Coordinates(coor.getRow(), coor.getCol() + WATCHSIZE));
		} else if (direction == 1) {
			tile = new Tile(
					new Coordinates(coor.getRow(), coor.getCol() - WATCHSIZE));
		} else if (direction == 2) {
			tile = new Tile(
					new Coordinates(coor.getRow() + WATCHSIZE, coor.getCol()));
		} else if (direction == 3) {
			tile = new Tile(
					new Coordinates(coor.getRow() - WATCHSIZE, coor.getCol()));
		}

		int row = tile->getCoordinates().getRow();
		int col = tile->getCoordinates().getCol();

		if (row >= 0 && row < mapData->getNRows() && col >= 0
				&& col < mapData->getNCols()) {
			mapData->moveMobileEntity(this->entity, tile);
		}

		if (tile)
			delete tile;
	}
}

MobileEntity* ArtificialIntelligence::getMobileEntity() {
	return entity;

}

ArtificialIntelligence::~ArtificialIntelligence() {

}


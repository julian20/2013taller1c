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

	if (entity->getAttackToEntity() != NULL && !entity->isDead()) {
		Tile* currentTile = entity->getTile();
		Tile* enemyTile = entity->getAttackToEntity()->getTile();

		if (mapData->distBetweenTilesInTiles(currentTile, enemyTile) < WATCHSIZE)
			return;
	}

	if (this->isAnyEnemyClose(mapData) && !entity->isDead()) {
		Entity& enemy = this->getNearestEnemy();
		entity->attackTo(&enemy);
	} else {
		if (entity->getClassName() == "MobileEntity") {
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
		if (mob->getTeam() != this->entity->getTeam())
			return true;
	}
	return false;
	//return false; // TODO :cambiar este harcoding
}

Entity& ArtificialIntelligence::getNearestEnemy() {

	MobileEntity* mob;
	list<MobileEntity*>::iterator iter;
	for (iter = entitiesNear.begin(); iter != entitiesNear.end(); ++iter) {
		mob = *iter;
		if (mob->getTeam() != this->entity->getTeam())
			return *mob;
	}

	return *mob;
}

void ArtificialIntelligence::watch(MapData* mapData) {

	Coordinates coor = this->entity->getCoordinates();

	Tile* tile;
	//Si no se esta moviendo o atacando busco a otro contrincante
	if (!entity->IsMoving() && !entity->isDead()) {
		int direction = rand() % 4;
		if (direction == 0) {
			//mobile->moveTo(coor.getCol(),coor.getRow()+WATCHSIZE);
			tile = new Tile(
					new Coordinates(coor.getRow(), coor.getCol() + WATCHSIZE));

		} else if (direction == 1) {
			//mobile->moveTo(coor.getCol(),coor.getRow()-WATCHSIZE);
			tile = new Tile(
					new Coordinates(coor.getRow(), coor.getCol() - WATCHSIZE));
		} else if (direction == 2) {
			//mobile->moveTo(coor.getCol()+WATCHSIZE,coor.getRow());
			tile = new Tile(
					new Coordinates(coor.getRow() + WATCHSIZE, coor.getCol()));
		} else if (direction == 3) {
			//mobile->moveTo(coor.getCol()-WATCHSIZE,coor.getRow());
			tile = new Tile(
					new Coordinates(coor.getRow() - WATCHSIZE, coor.getCol()));
		}
		if (tile->getCoordinates().getRow() > 0
				&& tile->getCoordinates().getRow() < mapData->getNRows()) {
			if (tile->getCoordinates().getCol() > 0
					&& tile->getCoordinates().getCol() < mapData->getNCols()) {
				mapData->moveMobileEntity((Mob*) this->entity, tile);
			}
		}
		if (tile)
			delete tile;
	}
}

ArtificialIntelligence::~ArtificialIntelligence() {

}


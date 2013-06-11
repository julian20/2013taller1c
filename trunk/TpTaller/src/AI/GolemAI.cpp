/*
 * GolemAI.cpp
 *
 *  Created on: 11/06/2013
 *      Author: tomas
 */

#include <AI/GolemAI.h>

#define WATCHSIZE 8
#define PLAYER_MAX_DIST 15

GolemAI::GolemAI() {
	// TODO Auto-generated constructor stub

}

void GolemAI::update(MapData* mapData) {

	if (!entity->isDead() && !entity->isFrozen()) {
		Tile* currentTile = entity->getTile();
		Tile* ownerTile = ((Golem*)entity)->getOwner()->getTile();
		int playerDistance = MapData::distBetweenTilesInTiles(ownerTile,currentTile);


		if (playerDistance > PLAYER_MAX_DIST){
			int x = Tile::computePosition(ownerTile->getCoordinates().getRow(),ownerTile->getCoordinates().getCol())->getX();
			int y = Tile::computePosition(ownerTile->getCoordinates().getRow(),ownerTile->getCoordinates().getCol())->getY();
			int z = Tile::computePosition(ownerTile->getCoordinates().getRow(),ownerTile->getCoordinates().getCol())->getZ();
			entity->moveTo(x,y,z);
		}

		if (entity->getAttackToEntity() != NULL) {

			Tile* enemyTile = entity->getAttackToEntity()->getTile();
			if (mapData->distBetweenTilesInTiles(currentTile,enemyTile) < WATCHSIZE && playerDistance < PLAYER_MAX_DIST)
				return;
		}

		if (this->isAnyEnemyClose(mapData)) {
			if (playerDistance > PLAYER_MAX_DIST) return;
			Entity& enemy = this->getNearestEnemy();
			entity->attackTo(&enemy);
		} else {
			if (entity->getClassName() == "MobileEntity") {
				entity->cancelAttack();
				this->watch(mapData);
			}
		}
	}
}

GolemAI::~GolemAI() {
	// TODO Auto-generated destructor stub
}


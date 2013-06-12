/*
 * GolemAI.cpp
 *
 *  Created on: 11/06/2013
 *      Author: tomas
 */

#include <AI/GolemAI.h>

#define WATCHSIZE 4
#define PLAYER_MAX_DIST 6

GolemAI::GolemAI() {
	// TODO Auto-generated constructor stub

}

void GolemAI::update(MapData* mapData) {
	if (!entity->isDead() && !entity->isFrozen()) {
		Tile* currentTile = entity->getTile();
		Tile* ownerTile = ((Golem*)entity)->getOwner()->getTile();
		int playerDistance = MapData::distBetweenTilesInTiles(ownerTile,currentTile);


		if (playerDistance > PLAYER_MAX_DIST){
			entity->assignPath(mapData->getPath(currentTile, ownerTile), mapData);
			entity->attackTo(NULL);
		} else {

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
}

GolemAI::~GolemAI() {
	// TODO Auto-generated destructor stub
}


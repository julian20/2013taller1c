/*
 * TileData.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: lucas
 */

#include <model/map/TileData.h>
#include <stdio.h>

using namespace std;

TileData::TileData() {
	entity = NULL;
	tileType = "neutral";
	iterator = entities.begin();
	wasVisible = false;
	isVisible = false;
	walkable = true;
}

std::string TileData::getType() {
	return tileType;
}

std::list<Entity*> TileData::getEntities() {
	return entities;
}

MobileEntity* TileData::getAttackableMobileEntity() {
	MobileEntity* mobileEntity;

	list<MobileEntity *>::const_iterator iter;
	for (iter = mobileEntities.begin(); iter != mobileEntities.end(); ++iter) {
		mobileEntity = *iter;

		if (mobileEntity->isAttackable())
			return mobileEntity;
	}

	return NULL;
}

Entity* TileData::getNextEntity() {
	return entity;
}

int TileData::getNumberOfEntitiesOnTile() {
	return entities.size();
}

bool TileData::getWasVisible() {
	return wasVisible;
}

bool TileData::getIsVisible() {
	return isVisible;
}

bool TileData::isWalkable(bool ignoremobileEntity) {
	bool allEntitiesWalkable = true;
	MobileEntity* current;
	list<MobileEntity *>::const_iterator iter;
	for (iter = mobileEntities.begin(); iter != mobileEntities.end(); ++iter) {
		current = *iter;

		if ( current->isWalkable() == false)
			allEntitiesWalkable = false;
	}

	if (ignoremobileEntity == true && walkable == true)
		return true;

	if (walkable == false)
		return false;
	return allEntitiesWalkable;
}

void TileData::setType(std::string type) {
	tileType = type;
}

void TileData::addMobileEntity(MobileEntity* newMobileEntity) {
	mobileEntities.push_back(newMobileEntity);
}

void TileData::removeMobileEntity(MobileEntity* mobileEntity) {
	mobileEntities.remove(mobileEntity);
}

void TileData::setVisibility(bool value) {
	if (value) {
		wasVisible = true;
		isVisible = true;
	} else {
		isVisible = false;
	}
}

void TileData::setWalkable(bool _walkable) {
	this->walkable = _walkable;
}

void TileData::addEntity(Entity* entity) {
	//Se agrega la entidad al final del vector.
	entities.push_back(entity);
	this->entity = entity;
}

void TileData::removeEntity(Entity* entity) {
	entities.remove(entity);
}

TileData::~TileData() {
	// TODO Auto-generated destructor stub
}

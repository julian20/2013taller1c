/*
 * PersistentConfiguration.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/persistence/PersistentConfiguration.h>

PersistentConfiguration::PersistentConfiguration() {
	this->textureHolder = NULL;
	this->mapData = NULL;
}

PersistentConfiguration::~PersistentConfiguration() {
	delete this->textureHolder;
	delete this->mapData;
	for (unsigned int i = 0; i < this->entityList.size(); i++) {
		delete this->entityList[i];
	}
}

TextureHolder* PersistentConfiguration::getTextureHolder() {
	return this->textureHolder;
}

void PersistentConfiguration::setTextureHolder(TextureHolder* textureHolder) {
	this->textureHolder = textureHolder;
}

std::vector<Entity*> PersistentConfiguration::getEntityList() {
	return this->entityList;
}

void PersistentConfiguration::setEntityList(std::vector<Entity*> entityList) {
	this->entityList = entityList;
}

MapData* PersistentConfiguration::getMapData() {
	return this->mapData;
}

void PersistentConfiguration::setMapData(MapData* mapData) {
	this->mapData = mapData;
}

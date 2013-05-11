/*
 * ServerMapPersistentConfiguration.cpp
 *
 *  Created on: May 11, 2013
 *      Author: gonchub
 */

#include <model/persistence/ServerMapPersistentConfiguration.h>

ServerMapPersistentConfiguration::ServerMapPersistentConfiguration() {
	this->textureHolder = NULL;
	this->mapData = NULL;
	this->animationConfiguration = NULL;
	this->entityViewMap = NULL;
}

ServerMapPersistentConfiguration::~ServerMapPersistentConfiguration() {
	delete this->textureHolder;
	delete this->mapData;
	delete this->animationConfiguration;
	delete this->entityViewMap;
}

TextureHolder* ServerMapPersistentConfiguration::getTextureHolder() {
	return this->textureHolder;
}

void ServerMapPersistentConfiguration::setTextureHolder(TextureHolder* textureHolder) {
	this->textureHolder = textureHolder;
}

MapData* ServerMapPersistentConfiguration::getMapData() {
	return this->mapData;
}

void ServerMapPersistentConfiguration::setMapData(MapData* mapData) {
	this->mapData = mapData;
}

GameConfiguration* ServerMapPersistentConfiguration::getAnimationConfiguration() {
	return this->animationConfiguration;
}

void ServerMapPersistentConfiguration::setAnimationConfiguration(
		GameConfiguration* aConfig) {
	this->animationConfiguration = aConfig;
}

EntityViewMap* ServerMapPersistentConfiguration::getEntityViewMap() {
	return this->entityViewMap;
}

void ServerMapPersistentConfiguration::setEntityViewMap(EntityViewMap* entityViewMap) {
	this->entityViewMap = entityViewMap;
}

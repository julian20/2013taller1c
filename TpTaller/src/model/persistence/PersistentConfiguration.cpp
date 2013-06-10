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
	this->animationConfiguration = NULL;
	this->entityViewMap = NULL;
	this->itemViewHolder = NULL;
}

PersistentConfiguration::~PersistentConfiguration() {
	delete this->mapData;
	delete this->animationConfiguration;
	delete this->entityViewMap;
	for (unsigned int i = 0; i < this->entityList.size(); i++) {
		delete this->entityList[i];
	}
	delete this->itemViewHolder;
}

TextureHolder* PersistentConfiguration::getTextureHolder() {
	return this->textureHolder;
}

void PersistentConfiguration::setTextureHolder(TextureHolder* textureHolder) {
	this->textureHolder = textureHolder;
}

std::vector<Player*> PersistentConfiguration::getPersonajeList() {
	return this->entityList;
}

void PersistentConfiguration::setPersonajeList(
		std::vector<Player*> entityList) {
	this->entityList = entityList;
}

MapData* PersistentConfiguration::getMapData() {
	return this->mapData;
}

void PersistentConfiguration::setMapData(MapData* mapData) {
	this->mapData = mapData;
}

GameConfiguration* PersistentConfiguration::getAnimationConfiguration() {
	return this->animationConfiguration;
}

void PersistentConfiguration::setAnimationConfiguration(
		GameConfiguration* aConfig) {
	this->animationConfiguration = aConfig;
}

EntityViewMap* PersistentConfiguration::getEntityViewMap() {
	return this->entityViewMap;
}

void PersistentConfiguration::setEntityViewMap(EntityViewMap* entityViewMap) {
	this->entityViewMap = entityViewMap;
}
void PersistentConfiguration::setMobileEntities(std::vector<MobileEntity*> mobiles)
{
	this->mobs= mobiles;
	for (unsigned int i = 0; i < this->mobs.size(); i++) {
			mobileEntityMap[i]= mobs[i];
		}

}
map<int,MobileEntity*> PersistentConfiguration::getMobileEntities()
{
	return this->mobileEntityMap;
}

void PersistentConfiguration::setViewList(std::vector<PlayerView*> viewList) {
	this->viewList = viewList;
}

std::vector<PlayerView*> PersistentConfiguration::getViewList() {
	return viewList;
}

void PersistentConfiguration::setMobileEntityViewList(std::vector<MobileEntityView*> viewList) {
	this->mobileEntityViewList = viewList;
}
void PersistentConfiguration::setMobileEntitiesView(std::vector<MobileEntityView*> list)
{
	this->allViewEntities = list;
}
std::vector<MobileEntityView*> PersistentConfiguration::getMobileEntitiesView()
{
	return this->allViewEntities;
}
std::vector<MobileEntityView*> PersistentConfiguration::getMobileEntityViewList() {
	return mobileEntityViewList;
}

std::vector<Item*> PersistentConfiguration::getItems() {
	return itemList;
}

void PersistentConfiguration::setItems(std::vector<Item*> items) {
	itemList = items;
}

ItemViewHolder* PersistentConfiguration::getItemViewHolder() {
	return itemViewHolder;
}

void PersistentConfiguration::setItemViewHolder(ItemViewHolder* itemViewHolder) {
	this->itemViewHolder = itemViewHolder;
}

MobileEntityViewHolder* PersistentConfiguration::getMobileEntityViewHolder() {
	return mobileEntityViewHolder;
}

void PersistentConfiguration::setMobileEntityViewHolder(MobileEntityViewHolder* mobileEntityViewHolder) {
	this->mobileEntityViewHolder = mobileEntityViewHolder;
}

vector<MobileEntity*> PersistentConfiguration::getMobileEntityList(){
	return mobs;
}

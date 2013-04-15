/*
 * EntityHolder.cpp
 *
 *  Created on: 13/04/2013
 *      Author: damian
 */

#include <view/entities/EntityHolder.h>

EntityHolder::EntityHolder() {
	this->auxiliarEntity=NULL;
}

void EntityHolder::addEntityViewData(EntityViewData* entityViewData)
{
	if(! this->duplicateEntity(entityViewData->getEntityId()))
	{
		delete entityViewData;
	}else
	{
		this->listOfEntities.push_back(entityViewData);
	}
}

EntityViewData* EntityHolder::getEntity(string id)
{
	for (unsigned int i = 0; i < this->listOfEntities.size(); i++) {
			EntityViewData* auxEntity = this->listOfEntities[i];
			if (id.compare(auxEntity->getEntityId()) == 0) {
				return auxEntity;
			}
		}

		//return this->auxiliarEntity;
	//por ahora devuelve Null luego tendria q mandar un objeto Auxiliar
	return NULL;
}

bool EntityHolder::duplicateEntity(string id)
{
	for (unsigned int i = 0; i < this->listOfEntities.size(); i++) {
			EntityViewData* auxEntityView = this->listOfEntities[i];
			if (id.compare(auxEntityView->getEntityId()) == 0) {
				return true;
			}
		}
		return false;
}

EntityHolder::~EntityHolder() {
	for (unsigned int i = 0; i < listOfEntities.size(); i++) {
			delete listOfEntities[i];
	}
}


/*
 * EntityHolder.h
 *
 *  Created on: 13/04/2013
 *      Author: damian
 */

#ifndef ENTITYHOLDER_H_
#define ENTITYHOLDER_H_
#include "string.h"
#include <vector>
#include <view/entities/EntityViewData.h>

using namespace std;
class EntityHolder {
public:
	EntityHolder();
	virtual ~EntityHolder();
	void addEntityViewData(EntityViewData* entityViewData);
	EntityViewData* getEntity(std::string id);
private:
	std::vector<EntityViewData*> listOfEntities;
	EntityViewData* auxiliarEntity;
	bool duplicateEntity(std::string id);
};

#endif /* ENTITYHOLDER_H_ */

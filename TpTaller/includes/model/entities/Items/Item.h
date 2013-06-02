/*
 * Item.h
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#ifndef ITEM_H_
#define ITEM_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>

using namespace std;

class Item: public Entity {
public:
	Item();
	virtual string getClassName();
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& mobileEntity);
	virtual ~Item();
};

#endif /* ITEM_H_ */

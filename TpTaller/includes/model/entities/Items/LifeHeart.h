/*
 * LifeHeart.h
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#ifndef LIFEHEART_H_
#define LIFEHEART_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>

using namespace std;

class LifeHeart: public Item {
public:
	LifeHeart();
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~LifeHeart();
};

#endif /* LIFEHEART_H_ */

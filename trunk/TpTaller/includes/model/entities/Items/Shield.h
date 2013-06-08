/*
 * Shield.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef SHIELD_H_
#define SHIELD_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>

class Shield: public Item {
public:
	Shield();
	Shield(Item* entity);
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~Shield();
};

#endif /* SHIELD_H_ */

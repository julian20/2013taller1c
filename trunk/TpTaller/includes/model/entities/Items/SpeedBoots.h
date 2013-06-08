/*
 * SpeedBoots.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef SPEEDBOOTS_H_
#define SPEEDBOOTS_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>

class SpeedBoots: public Item {
public:
	SpeedBoots();
	SpeedBoots(Item* entity);
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~SpeedBoots();
};

#endif /* SPEEDBOOTS_H_ */

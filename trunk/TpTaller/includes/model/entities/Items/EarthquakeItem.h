/*
 * EarthquakeItem.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef EARTHQUAKEITEM_H_
#define EARTHQUAKEITEM_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>

class EarthquakeItem: public Item {
public:
	EarthquakeItem();
	EarthquakeItem(Item* entity);
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~EarthquakeItem();
};

#endif /* EARTHQUAKEITEM_H_ */

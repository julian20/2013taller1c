/*
 * EarthquakeItem.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef EARTHQUAKEITEM_H_
#define EARTHQUAKEITEM_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>
#include <model/entities/player/Player.h>

class EarthquakeItem: public Item {
public:
	EarthquakeItem();
	EarthquakeItem(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~EarthquakeItem();
};

#endif /* EARTHQUAKEITEM_H_ */

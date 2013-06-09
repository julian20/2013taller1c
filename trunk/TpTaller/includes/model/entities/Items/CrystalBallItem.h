/*
 * EarthquakeItem.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef CRYSTALBALLEITEM_H_
#define CRYSTALBALLEITEM_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class CrystalBallItem: public Item {
public:
	CrystalBallItem();
	CrystalBallItem(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~CrystalBallItem();
};

#endif /* CRYSTALBALLEITEM_H_ */

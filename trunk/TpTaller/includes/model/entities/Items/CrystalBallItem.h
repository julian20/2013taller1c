/*
 * EarthquakeItem.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef CRYSTALBALLEITEM_H_
#define CRYSTALBALLEITEM_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/Items/Item.h>

class CrystalBallItem: public Item {
public:
	CrystalBallItem();
	CrystalBallItem(Item* entity);
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	virtual ~CrystalBallItem();
};

#endif /* CRYSTALBALLEITEM_H_ */

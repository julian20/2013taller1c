/*
 * FrostWandItem.h
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#ifndef FREEZESPELLITEM_H_
#define FREEZESPELLITEM_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class FreezeSpellItem: public Item {
public:
	FreezeSpellItem();
	FreezeSpellItem(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~FreezeSpellItem();
};

#endif /* FREEZESPELLITEM_H_ */

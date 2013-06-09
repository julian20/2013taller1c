/*
 * SpellShieldItem.h
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#ifndef SPELLSHIELDITEM_H_
#define SPELLSHIELDITEM_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class SpellShieldItem: public Item {
public:
	SpellShieldItem();
	SpellShieldItem(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~SpellShieldItem();
};

#endif /* SPELLSHIELDITEM_H_ */

/*
 * GolemSpellItem.h
 *
 *  Created on: 11/06/2013
 *      Author: tomas
 */

#ifndef GOLEMSPELLITEM_H_
#define GOLEMSPELLITEM_H_

#include <model/entities/Items/Item.h>

class Player;

using namespace std;

class GolemSpellItem: public Item {
public:
	GolemSpellItem();
	GolemSpellItem(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~GolemSpellItem();
};

#endif /* GOLEMSPELLITEM_H_ */

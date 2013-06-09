/*
 * Chest.h
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#ifndef CHEST_H_
#define CHEST_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class Chest: public Item {
public:
	Chest();
	Chest(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~Chest();
};

#endif /* CHEST_H_ */

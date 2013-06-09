/*
 * Shield.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef SHIELD_H_
#define SHIELD_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class Shield: public Item {
public:
	Shield();
	Shield(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~Shield();
};

#endif /* SHIELD_H_ */

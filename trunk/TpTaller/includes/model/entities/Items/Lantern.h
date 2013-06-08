/*
 * Lantern.h
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#ifndef LANTERN_H_
#define LANTERN_H_

#include <model/entities/Entity.h>
#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class Lantern: public Item {
public:
	Lantern();
	Lantern(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~Lantern();
};

#endif /* LANTERN_H_ */

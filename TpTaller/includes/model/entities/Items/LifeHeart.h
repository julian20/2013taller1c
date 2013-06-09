/*
 * LifeHeart.h
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#ifndef LIFEHEART_H_
#define LIFEHEART_H_

#include <model/entities/Items/Item.h>

class Player;

using namespace std;

class LifeHeart: public Item {
public:
	LifeHeart();
	LifeHeart(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~LifeHeart();
};

#endif /* LIFEHEART_H_ */

/*
 * StrengthBuff.h
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#ifndef STRENGTHBUFF_H_
#define STRENGTHBUFF_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class StrengthBuff: public Item {
public:
	StrengthBuff();
	StrengthBuff(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~StrengthBuff();
};

#endif /* STRENGTHBUFF_H_ */

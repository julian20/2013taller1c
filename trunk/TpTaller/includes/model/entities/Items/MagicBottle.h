/*
 * MagicBottle.h
 *
 *  Created on: 02/06/2013
 *      Author: damian
 */

#ifndef MAGICBOTTLE_H_
#define MAGICBOTTLE_H_

#include <model/entities/player/Player.h>
#include <model/entities/Entity.h>

using namespace std;

class MagicBottle : public Item {
public:
	MagicBottle();
	MagicBottle(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~MagicBottle();
};

#endif /* MAGICBOTTLE_H_ */

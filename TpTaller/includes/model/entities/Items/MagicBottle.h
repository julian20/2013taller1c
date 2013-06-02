/*
 * MagicBottle.h
 *
 *  Created on: 02/06/2013
 *      Author: damian
 */

#ifndef MAGICBOTTLE_H_
#define MAGICBOTTLE_H_
#include <string>
#include <model/entities/Items/Item.h>
#include <model/entities/Entity.h>

using namespace std;

class MagicBottle : public Item {
public:
	MagicBottle();
	virtual void applyEffects(Entity* entity);
	//virtual void applyEffects(MobileEntity* entity);
	virtual string getClassName();
	virtual ~MagicBottle();
};

#endif /* MAGICBOTTLE_H_ */

/*
 * MapItem.h
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#ifndef MAPITEM_H_
#define MAPITEM_H_

#include <model/entities/player/Player.h>
#include <model/entities/Items/Item.h>

class MapItem: public Item {
public:
	MapItem();
	MapItem(Item* entity);
	virtual void applyEffects(Player& entity);
	virtual void collideTo(Player& entity);
	virtual string getClassName();
	virtual ~MapItem();
};

#endif /* MAPITEM_H_ */

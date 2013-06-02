/*
 * Item.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/Item.h>

Item::Item() {

}

string Item::getClassName() {
	return "Item";
}

void Item::applyEffects(MobileEntity* entity) {}

void Item::applyEffects(Entity* entity) {}

Item::~Item() {

}


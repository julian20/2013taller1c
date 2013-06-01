/*
 * Item.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/Item.h>

Item::Item() {

}

string Item::getClassName() {
	return "Item";
}

void Item::applyEffects(Entity* entity) {
	// TODO: algun atributo o define de vida original.
	entity->setLife(100);
}

void Item::applyEffects(MobileEntity* entity) {
	entity->setSpeedMagnitude(entity->getSpeed()->getMagnitude() * 2);
}

Item::~Item() {

}


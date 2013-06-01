/*
 * LifeHeart.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/LifeHeart.h>

LifeHeart::LifeHeart() {
}

void LifeHeart::applyEffects(Entity* entity) {
	// TODO: algun atributo o define de vida original.
	entity->setLife(100);
}

void LifeHeart::applyEffects(MobileEntity* entity) {
	entity->setLife(100);
}

string LifeHeart::getClassName() {
	return "LifeHeart";
}

LifeHeart::~LifeHeart() {
}


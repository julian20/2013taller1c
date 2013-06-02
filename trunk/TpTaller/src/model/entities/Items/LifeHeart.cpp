/*
 * LifeHeart.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/LifeHeart.h>

LifeHeart::LifeHeart() {
}

void LifeHeart::applyEffects(Entity& entity) {
	// TODO: algun atributo o define de vida original.
	entity.setLife(100);
}

void LifeHeart::applyEffects(MobileEntity& entity) {
	entity.setLife(100);
}

void LifeHeart::collideTo(MobileEntity& entity) {
	applyEffects(entity);
}

string LifeHeart::getClassName() {
	return "LifeHeart";
}

LifeHeart::~LifeHeart() {
}

